✅ Here’s the **full step-by-step procedure** for generating your own **GEMINI_API_KEY** to use in your project:

---

# 🎯 How to Generate Your Google Gemini API Key (Step-by-Step)

### 1. **Go to Google AI Studio**
- Visit: [https://aistudio.google.com/](https://aistudio.google.com/)

---

### 2. **Sign in with your Google Account**
- Login with your personal or work Gmail.
- Accept the terms and conditions.

---

### 3. **Create an API Key**
- Once logged into AI Studio:
  - On the left sidebar, find and click on **"API Keys"**.
  - Click **"Create API Key"**.
  - Give it any name you want (e.g., `Gemini-API-Key-For-YOLO`).
  - Copy the API key shown.

---
  
### 4. **Replace the API Key in Your Code**

In your project (`yolo_gemini_gui.py`), find this line:

```python
GEMINI_API_KEY = "your-api-key-here"
```
Replace it with the key you copied.

Example:

```python
GEMINI_API_KEY = "AIzaSyA*******your-own-key********"
```

✅ You don't need to modify `GEMINI_URL` — it automatically attaches your key:

```python
GEMINI_URL = f"https://generativelanguage.googleapis.com/v1beta/models/gemini-2.0-flash:generateContent?key={GEMINI_API_KEY}"
```

---

# 🧠 Important Notes

| Important | Details |
|:---|:---|
| **Free Usage** | Google currently gives free API usage quotas (limited tokens per month). |
| **Don't share API key publicly** | Hide it if uploading to GitHub (e.g., use `.env` file if needed). |
| **Model Used** | You are calling `gemini-2.0-flash`, perfect for lightweight tasks like object description! |

---

# 📋 Quick Summary
| Step | Action |
|:---|:---|
| 1 | Visit [AI Studio](https://aistudio.google.com/) |
| 2 | Create an API Key |
| 3 | Copy and paste it inside your project |
| 4 | Start running your code 🚀 |

---
