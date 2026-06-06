import os
import time
from flask import Flask, request

app = Flask(__name__)

SAVE_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "photos")
os.makedirs(SAVE_DIR, exist_ok=True)

@app.route('/upload', methods=['POST'])
def upload():
    ts = int(time.time() * 1000)
    filename = os.path.join(SAVE_DIR, f"photo_{ts}.jpg")
    with open(filename, 'wb') as f:
        f.write(request.data)
    size = len(request.data)
    print(f"Saved: photo_{ts}.jpg  ({size} bytes)")
    return f"Saved {size} bytes\n", 200

@app.route('/')
def index():
    files = sorted(os.listdir(SAVE_DIR), reverse=True)
    links = ''.join(f'<li><a href="/photos/{f}">{f}</a></li>' for f in files[:50])
    return f'<h3>Photos ({len(files)} total)</h3><ul>{links}</ul>'

@app.route('/photos/<filename>')
def photo(filename):
    from flask import send_from_directory
    return send_from_directory(SAVE_DIR, filename)

if __name__ == '__main__':
    print(f"Photos will be saved to: {SAVE_DIR}")
    print("Listening on http://0.0.0.0:8888")
    app.run(host='0.0.0.0', port=8888)
