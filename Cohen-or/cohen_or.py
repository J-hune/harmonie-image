from cohenOrC import *
import cv2
import numpy as np
import base64 # for socket encoding
from datetime import datetime

if __name__ == "__main__":
    img = cv2.imread("0.jpg", cv2.IMREAD_COLOR)
    hsv_img = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)


    startTime = datetime.now()
    ####

    scheme = find_minimizing_scheme(hsv_img)

    print(
        scheme[0].sectors,
        scheme[1]
    )

    print (datetime.now() - startTime )
    startTime = datetime.now()

    project_image_on_scheme(hsv_img, scheme)


    ####
    print (datetime.now() - startTime )


    res_img = cv2.cvtColor(hsv_img, cv2.COLOR_HSV2RGB)
    cv2.imwrite("result.jpeg", res_img)

    cv2.imshow('image',res_img)

    print("press 0 to close")
    cv2.waitKey(0)

    cv2.destroyAllWindows()
