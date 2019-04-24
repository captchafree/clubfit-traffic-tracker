import cv2
import numpy as np

def test():
	cap = cv2.VideoCapture("test_images/sample.mov")
	# Capture every 5 seconds
	# framerate = cap.get(50)

	fgbg = cv2.createBackgroundSubtractorMOG2()

	while(1):
		ret, frame = cap.read()
		fgmask = fgbg.apply(frame)

		cv2.rectangle(fgmask,(315,25),(400,175),(255,255,255),3)
		cropped = fgmask[25:175, 315:400]
		mean = np.mean(cropped, axis=(0, 1))

		# Threshold value
		if mean > 30:
			print "Object Detected (% 255 = {})".format(mean)

		cv2.imshow('frame',fgmask)
		k = cv2.waitKey(30) & 0xff
		if k == 27:
			break

	cap.release()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	test()
