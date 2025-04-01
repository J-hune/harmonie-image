import cython
import numpy as np

def get_hue(p: np.array) -> float:
    return p[0] * 2  # because opencv uses hues in [0, 127]


def angle_distance(a: float, b: float):
    a = a % 360
    b = b % 360
    return min(
        abs(a - b),
        abs(a + 360 - b)
    )


def between(v: float, a: float, b: float)-> bool:
    v = v % 360
    a = a % 360
    b = b % 360
    if b > a: return a <= v <= b

    return (v >= a) or (v <= b)


class HarmonicTemplate:
    sectors:list[tuple[float, float]] = []

    def __init__(self, sectors:list[tuple[float, float]]):
        self.sectors = sectors


def E(template:HarmonicTemplate, alpha:float, p:np.array) -> float:  # p is an opencv hsv pixel. hue is half range

    best_hue:float = 0
    best_dist: float = 99999.0
    for (wideness, angle_to_sector) in template.sectors:
        min_bound:float = (angle_to_sector + alpha - wideness / 2.0) % 360
        max_bound:float = (angle_to_sector + alpha + wideness / 2.0) % 360

        hue:float = get_hue(p)

        if between(hue, min_bound, max_bound): return hue  # if we're inside the sector

        dist = angle_distance(hue, min_bound)
        if dist < best_dist:
            best_dist = dist
            best_hue = min_bound

        dist = angle_distance(hue, max_bound)
        if dist < best_dist:
            best_dist = dist
            best_hue = max_bound

    return best_hue

def arc_length(p) -> float:  # pi r theta / 180
    # saturation = p[1]

    return np.pi * p / 180.0


## "distance" function
## X is an image
## (template, alpha) is an harmonic scheme: a template offset by an angle
def F(X, template, alpha) -> float:
    acc = 0

    for i in range(0, X.shape[0]):
        for j in range(0, X.shape[1]):
            p = X[i, j]

            closest_hue = E(template, alpha, p)
            acc += arc_length(abs(get_hue(p) - closest_hue)) * p[1] / 255.0
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

def find_minimizing_scheme(X):
    best_template:HarmonicTemplate
    best_angle:float = 0
    best_distance:float = np.inf
    for m in templates.keys():
        print("processing template ", m)
        for alpha in range(0, 359, 36):
            print("     processing angle ", alpha)
            d:float = F(X, templates.__getitem__(m), alpha)
            print(d)
            if d < best_distance:
                best_distance = d
                best_template = templates.__getitem__(m)
                best_angle = alpha
    return (best_template, best_angle)


def project_image_on_scheme(X:np.array, scheme:tuple[HarmonicTemplate, float]) -> None:
    for i in range(0, X.shape[0]):
        for j in range(0, X.shape[1]):
            X[i, j][0] = E(scheme[0], scheme[1], X[i, j])