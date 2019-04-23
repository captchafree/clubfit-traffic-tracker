import cv2
import numpy as np

def manual():
	# Load images
	original = cv2.imread("test_images/original.jpeg", 0)
	img = cv2.imread("test_images/single_card.jpeg", 0)

    # Draw bounding rect
	# cv2.rectangle(img,(2000,1000),(2300,1400),(0,0,255),3)
	# cv2.imwrite("test_images/bounding_box.png", img)

	# Crop image
	cropped_original = original[1000:1400, 2000:2300]
	cv2.imwrite("test_images/cropped_original.png", cropped_original)

	cropped = img[1000:1400, 2000:2300]
	cv2.imwrite("test_images/cropped.png", cropped)

	# Add blur?

	# Subtract images
	fgbg = cv2.createBackgroundSubtractorMOG2()
	result = fgbg.apply(original)
	result = fgbg.apply(img)
	t = cv2.imread("test_images/grid_1.jpeg", 0)
	result = fgbg.apply(t)
	t = cv2.imread("test_images/grid_2.jpeg", 0)
	result = fgbg.apply(t)
	cv2.imwrite("test_images/bg_sub.png", result)

	blur = cv2.GaussianBlur(cropped_original,(5,5),0)
	diff = blur - cropped_original
	cv2.imwrite("test_images/diff_without_card.png", diff)

	diff = blur - cropped
	cv2.imwrite("test_images/diff_with_card.png", diff)

	# Count white pixels
	n_white_pix = np.sum(diff >= 150)
	print n_white_pix
	print diff.size

	percent_white = n_white_pix / diff.size
	print percent_white

def test():
	cap = cv2.VideoCapture("test_images/sample.mov")
	# Capture every 5 seconds
	framerate = cap.get(50)

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
