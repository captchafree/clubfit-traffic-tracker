from SimpleCV import Image, Camera
import json
import pygame
import pygame.camera
from flask import Flask

app = Flask(__name__)

@app.route('/')
def test_server():
	try:
		cam = Camera()
		img = cam.getImage()
		img.save("snapshot.jpg")

		response = dict()
		response["code"] = 200
		response["image"] = img
		
		return json.dumps(response)
	except Exception as e:
		return "<h1>Image read failed...{}</h1>".format(str(e))

if __name__ == '__main__':
	app.run(debug=True, port=5000, host='0.0.0.0')
