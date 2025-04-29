import tkinter as tk
from tkinter import Label, Button
import cv2
import os
import base64
import requests
import json
from PIL import Image, ImageTk
from ultralytics import YOLO
import pyttsx3
import speech_recognition as sr
import threading

# ======= SETUP =======
GEMINI_API_KEY = "you key"
GEMINI_URL = f"your url"
IMAGE_PATH = "captured.jpg"

engine = pyttsx3.init()
recognizer = sr.Recognizer()
model = YOLO("yolov8s.pt")
cap = cv2.VideoCapture(0)

# ======= FUNCTIONS =======
def detect_objects(image):
    results = model(image)[0]
    labels = [model.names[int(cls)] for cls in results.boxes.cls]
    unique_labels = list(set(labels))
    return unique_labels, results

def image_to_base64(filepath):
    with open(filepath, "rb") as img_file:
        return base64.b64encode(img_file.read()).decode("utf-8")

def speak_and_listen(prompt):
    engine.say(prompt)
    engine.runAndWait()
    with sr.Microphone() as source:
        print("🎤 Listening for your command...")
        try:
            audio = recognizer.listen(source, timeout=5)
            command = recognizer.recognize_google(audio)
            print(f"You said: {command}")
            return command
        except Exception as e:
            print("Could not understand audio.", e)
            return ""

def send_to_gemini(base64_image, object_list):
    prompt_text = (
        "Describe only what the person is doing (if any), and describe the objects in the image briefly. "
        "Avoid mentioning clothing, gender, colors, or environment."
    )
    if object_list:
        prompt_text += f" These objects were detected: {', '.join(object_list)}."

    headers = {"Content-Type": "application/json"}
    data = {
        "contents": [
            {
                "parts": [
                    {"text": prompt_text},
                    {
                        "inline_data": {
                            "mime_type": "image/jpeg",
                            "data": base64_image
                        }
                    }
                ]
            }
        ]
    }

    response = requests.post(GEMINI_URL, headers=headers, data=json.dumps(data))
    if response.status_code == 200:
        result = response.json()
        try:
            caption = result["candidates"][0]["content"]["parts"][0]["text"]
            print("\n📝 Gemini Description:")
            print(caption)
            engine.say(caption)
            engine.runAndWait()
        except Exception as e:
            print("⚠ Could not parse Gemini response.", e)
    else:
        print(f"❌ Gemini API error: {response.status_code}")
        print(response.text)

# ======= GUI =======
app = tk.Tk()
app.title("Real-time Detection and Gemini Describer")
app.geometry("700x600")

label = Label(app, text="Live Stream + Gemini Description", font=("Arial", 16))
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
    app.after(100, update_frame)

# Ask and listen logic with retry

def ask_and_listen():
    response = speak_and_listen("Would you like to detect again?")
    if "yes" in response.lower():
        print("🔁 Retrying detection...")
        handle_detection()
    else:
        print("🛑 Detection stopped.")

# On-click function
def handle_detection():
    if os.path.exists(IMAGE_PATH):
        base64_img = image_to_base64(IMAGE_PATH)
        frame = cv2.imread(IMAGE_PATH)
        detected_objects, _ = detect_objects(frame)
        send_to_gemini(base64_img, detected_objects)
        threading.Thread(target=ask_and_listen).start()

btn_detect = Button(app, text="Detect and Describe", command=handle_detection, width=30, height=2)
btn_detect.pack(pady=20)

update_frame()
app.mainloop()
cap.release()
