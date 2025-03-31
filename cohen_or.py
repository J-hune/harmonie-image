
import cv2
import numpy as np
import base64 # for socket encoding
from datetime import datetime

def get_hue(p):
    return p[0] * 2 # because opencv uses hues in [0, 127]


def angle_distance(a, b):
    a = a % 360
    b = b % 360
    return min(
        abs(a - b),
        abs(a + 360 - b)
    )

def between(v, a, b):
    v = v % 360
    a = a % 360
    b = b % 360
    if b > a: return a <= v <= b

    return (v >= a) or (v <= b)



class HarmonicTemplate:
    sectors = []

    def __init__(self, sectors):
        self.sectors = sectors

def E(template, alpha, p): # p is an opencv hsv pixel. hue is half range

    best_hue = None
    best_dist = 99999
    for (wideness, angle_to_sector) in template.sectors:
        min_bound = (angle_to_sector + alpha - wideness / 2.0) % 360
        max_bound = (angle_to_sector + alpha + wideness / 2.0) % 360

        hue = get_hue(p)

        if between(hue, min_bound, max_bound): return hue # if we're inside the sector

        dist = angle_distance(hue, min_bound)
        if dist < best_dist:
            best_dist = dist
            best_hue = min_bound 

        dist = angle_distance(hue, max_bound)
        if dist < best_dist:
            best_dist = dist
            best_hue = max_bound
    
    return best_hue

# numpyzed
#def arc_length(p): # pi r theta / 180
#    saturation = np.array(p)[:,1]
#    hue = np.array(p)[:, 0]
#    return np.pi * saturation * hue / 180.0

def arc_length(p): # pi r theta / 180
    #saturation = p[1]

    return np.pi * p / 180.0


## "distance" function
## X is an image
## (template, alpha) is an harmonic scheme: a template offset by an angle
def F(X, template, alpha):
    acc = 0

    for i in range(0, X.shape[0]):
        for j in range(0, X.shape[1]):
            p = X[i, j]

            closest_hue = E(template, alpha, p)
            acc += arc_length(abs(get_hue(p) - closest_hue)) * p[1]/255.0
    return acc


templates = {
    "i": HarmonicTemplate([(18.0, 0)]),
    "V": HarmonicTemplate([(93.6, 0)]),
    "L": HarmonicTemplate([(18.0, 0), (72.2, 90.0)]),
    "I": HarmonicTemplate([(18.0, 0), (18.0, 180.0)]),
    "T": HarmonicTemplate([(180.0, 0)]),
    "Y": HarmonicTemplate([(93.6, 0), (18.0, 180.0)]),
    "X": HarmonicTemplate([(93.6, 0), (93.6, 180.0)])
}

img = cv2.imread("0.jpg", cv2.IMREAD_COLOR)



hsv_img = cv2.cvtColor(img, cv2.COLOR_RGB2HSV)


startTime = datetime.now()

print(F(
    hsv_img, 
    templates["V"],
    0
))


#cv2.imshow('image',img)

#print("press 0 to close")
print (datetime.now() - startTime )

cv2.waitKey(0)

cv2.destroyAllWindows()
