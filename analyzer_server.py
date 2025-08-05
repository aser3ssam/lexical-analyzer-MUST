from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
import subprocess

app = Flask(__name__, static_folder='assets')
CORS(app)

@app.route('/')
def serve_index():
    return send_from_directory('.', 'index.html')

@app.route('/assets/<path:filename>')
def serve_assets(filename):
    return send_from_directory('assets', filename)

@app.route('/analyze', methods=['POST'])
def analyze():
    print("✅ Received analyze request")
    code = request.json.get('code', '')
    try:
        result = subprocess.run(
            ['./analyzer'],
            input=code.encode(),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=5
        )
        output = result.stdout.decode() + result.stderr.decode()
        return jsonify({"output": output.strip()}), 200
    except subprocess.TimeoutExpired:
        return jsonify({"error": "Execution timed out"}), 500
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == '__main__':
    app.run(port=5000, debug=True)
