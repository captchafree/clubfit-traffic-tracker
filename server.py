import cv2
from flask import Flask, send_file

app = Flask(__name__)

@app.route('/')
def test_server():
	try:
		camera = cv2.VideoCapture(1)
		return_value, image = camera.read()
		cv2.imwrite('most_recent.png', image)
		return send_file('most_recent.png')
	except Exception as e:
		return "{}".format(str(e))

if __name__ == '__main__':
	app.run(debug=True, port=5000, host='0.0.0.0')