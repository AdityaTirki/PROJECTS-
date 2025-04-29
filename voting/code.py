import tkinter as tk
from tkinter import Label, Button
import cv2
import os
import openai
from PIL import Image, ImageTk
from ultralytics import YOLO
import pyttsx3

# ======= SETUP =======
OPENAI_API_KEY = "yourkey"
IMAGE_PATH = "captured.jpg"

openai.api_key = OPENAI_API_KEY

engine = pyttsx3.init()
model = YOLO("yolov8s.pt")

# Use CAP_DSHOW for Windows camera
cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)

# ======= FUNCTIONS =======
def detect_objects(image):
    results = model(image)[0]
    labels = [model.names[int(cls)] for cls in results.boxes.cls]
    unique_labels = list(set(labels))
    return unique_labels, results

def speak_text(text):
    engine.say(text)
    engine.runAndWait()

def send_to_chatgpt(object_list):
    prompt_text = (
        "Describe briefly what activities are happening and objects you see in the image. "
        "Detected objects are: " + ", ".join(object_list) + ". "
        "Avoid talking about colors, clothing, or background environment."
    )

    try:
        response = openai.ChatCompletion.create(
            model="gpt-4",
            messages=[
                {"role": "user", "content": prompt_text}
            ]
        )
        caption = response['choices'][0]['message']['content']
        print("\n📝 ChatGPT Description:")
        print(caption)
        speak_text(caption)
    except Exception as e:
        print("⚠️ OpenAI API error:", e)

# ======= GUI =======
app = tk.Tk()
app.title("Real-time Detection and ChatGPT Description")
app.geometry("700x600")

label = Label(app, text="Live Stream + ChatGPT Description", font=("Arial", 16))
label.pack(pady=10)

video_label = Label(app)
video_label.pack()

# Real-time update loop
def update_frame():
    ret, frame = cap.read()
    if ret:
        cv2.imwrite(IMAGE_PATH, frame)
        detected_objects, results = detect_objects(frame)
        annotated_frame = results.plot()
        img_rgb = cv2.cvtColor(annotated_frame, cv2.COLOR_BGR2RGB)
        img = Image.fromarray(img_rgb)
        imgtk = ImageTk.PhotoImage(image=img)
        video_label.imgtk = imgtk
        video_label.configure(image=imgtk)
    else:
        print("⚠️ Warning: Camera frame not grabbed.")
    app.after(100, update_frame)

# On-click function
def handle_detection():
    if os.path.exists(IMAGE_PATH):
        frame = cv2.imread(IMAGE_PATH)
        detected_objects, _ = detect_objects(frame)
        if detected_objects:
            send_to_chatgpt(detected_objects)
        else:
            print("⚠️ No objects detected to describe.")

btn_detect = Button(app, text="Detect and Describe", command=handle_detection, width=30, height=2)
btn_detect.pack(pady=20)

update_frame()
app.mainloop()
cap.release()
