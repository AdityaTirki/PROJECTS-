YOLOv8 + Gemini AI: Real-Time Voice-Activated Object DescriptionHere's the complete and updated `README.md` for your current project — with **no voice trigger**, **manual button detection**, and **Gemini response spoken aloud**:

---

## 🎯 YOLOv8 + Gemini 2.0 | Real-Time Object Detection and Scene Description

This project is a desktop GUI application that:

- 📷 Captures real-time webcam frames  
- 🔍 Detects objects using **YOLOv8**  
- 🧠 Sends the image to **Google Gemini 2.0 Flash** for scene description  
- 🔊 Speaks the AI-generated description using text-to-speech  

> Just click “Detect and Describe” to run live AI-powered detection.

---

### 🚀 Features

- 🖼️ Real-time webcam stream using OpenCV
- 🧠 Gemini AI generates natural language descriptions
- 🗣️ Text-to-speech (TTS) narration of Gemini’s response
- 🧠 Object detection via Ultralytics YOLOv8
- 🖱️ GUI-based trigger (no voice activation)

---

### 📦 Requirements

Install the dependencies:

```bash
pip install ultralytics opencv-python pyttsx3 pillow requests
```

---

### 🔑 Setup

1. Clone this repository
2. Replace the Gemini API key in `GEMINI_API_KEY`:

```python
GEMINI_API_KEY = "your-gemini-api-key"
```

You can generate a key from [Google AI Studio](https://aistudio.google.com/app/apikey).

---

### ▶️ How to Run

```bash
python yolo_gemini_gui.py
```

Then click the **“Detect and Describe”** button.  
It will:
1. Capture a frame from the webcam
2. Detect objects using YOLOv8
3. Send the frame to Gemini
4. Read the description aloud using text-to-speech

---

### 📁 Project Structure

```
📦 project-root/
├── yolo_gemini_gui.py       # main app file
├── captured.jpg             # auto-saved frame
├── README.md                # this file
```

---

### 📸 Screenshot

*(Add a screenshot of the GUI showing detection)*

---

### 🔐 Tech Stack

| Component      | Use                                  |
|----------------|---------------------------------------|
| Tkinter        | GUI layout                            |
| OpenCV         | Webcam stream + image capture         |
| Ultralytics YOLOv8 | Object detection                 |
| Gemini API     | Scene-level image understanding       |
| pyttsx3        | Offline text-to-speech engine         |
| Pillow         | Convert OpenCV image for GUI display  |

---

### ✅ Future Add-ons

- [ ] Voice activation (e.g., “Hey Google”)
- [ ] Display Gemini response on screen
- [ ] Logging of detected objects

