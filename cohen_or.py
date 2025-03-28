
import cv2
from Cython import typeof

def get_hue(p):
    return p[0] * 2


def angle_distance(a, b):
    a = a % 360
    b = b % 360
    return min(
        abs(a - b),
        abs(a + 360 - b)
    )

def on_left(a, b): # TODO
    a = a % 360
    b = b % 360
    return (abs(a - b) < abs(a + 360 - b) )



class HarmonicTemplate:
    sectors = []

    def __init__(self, sectors):
        self.sectors = sectors

    def E(self, alpha, p):
        for (wideness, angle_to_sector) in self.sectors:
            min_bound = angle_to_sector + alpha - wideness / 2.0
            max_bound = angle_to_sector + alpha + wideness / 2.0

            hue = get_hue(p)
            if min_bound <=









templates = {
    "i": HarmonicTemplate([(18.0, 0)]),
    "V": HarmonicTemplate([(93.6, 0)]),
    "L": HarmonicTemplate([(18.0, 0), (72.2, 90.0)]),
    "I": HarmonicTemplate([(18.0, 0), (18.0, 180.0)]),
    "T": HarmonicTemplate([(180.0, 0)]), # rotate or not?
    "Y": HarmonicTemplate([(93.6, 0), (18.0, 180.0)]),
    "X": HarmonicTemplate([(93.6, 0), (93.6, 180.0)])
}



img = cv2.imread("0.jpg", cv2.IMREAD_COLOR)

hsv_img = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)


print(hsv_img[10, 10])

cv2.imshow("image", img)


cv2.waitKey(0)

# It is for removing/deleting created GUI window from screen
# and memory
cv2.destroyAllWindows()
