from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
import os
import csv
"""
Usage
python3 convert.py

Will convert automatically everything under img_out to png
will also generate automatically the histograms for every .dat file in img_out
"""
# hues for hue histograms
colors = [(0.8,0.16,0.16), (0.8,0.175059,0.16), (0.8,0.190118,0.16), (0.8,0.205176,0.16), (0.8,0.220235,0.16), (0.8,0.235294,0.16), (0.8,0.250353,0.16), (0.8,0.265412,0.16), (0.8,0.280471,0.16), (0.8,0.295529,0.16), (0.8,0.310588,0.16), (0.8,0.325647,0.16), (0.8,0.340706,0.16), (0.8,0.355765,0.16), (0.8,0.370824,0.16), (0.8,0.385882,0.16), (0.8,0.400941,0.16), (0.8,0.416,0.16), (0.8,0.431059,0.16), (0.8,0.446118,0.16), (0.8,0.461176,0.16), (0.8,0.476235,0.16), (0.8,0.491294,0.16), (0.8,0.506353,0.16), (0.8,0.521412,0.16), (0.8,0.536471,0.16), (0.8,0.551529,0.16), (0.8,0.566588,0.16), (0.8,0.581647,0.16), (0.8,0.596706,0.16), (0.8,0.611765,0.16), (0.8,0.626824,0.16), (0.8,0.641882,0.16), (0.8,0.656941,0.16), (0.8,0.672,0.16), (0.8,0.687059,0.16), (0.8,0.702118,0.16), (0.8,0.717176,0.16), (0.8,0.732235,0.16), (0.8,0.747294,0.16), (0.8,0.762353,0.16), (0.8,0.777412,0.16), (0.8,0.792471,0.16), (0.792471,0.8,0.16), (0.777412,0.8,0.16), (0.762353,0.8,0.16), (0.747294,0.8,0.16), (0.732235,0.8,0.16), (0.717176,0.8,0.16), (0.702118,0.8,0.16), (0.687059,0.8,0.16), (0.672,0.8,0.16), (0.656941,0.8,0.16), (0.641882,0.8,0.16), (0.626824,0.8,0.16), (0.611765,0.8,0.16), (0.596706,0.8,0.16), (0.581647,0.8,0.16), (0.566588,0.8,0.16), (0.551529,0.8,0.16), (0.536471,0.8,0.16), (0.521412,0.8,0.16), (0.506353,0.8,0.16), (0.491294,0.8,0.16), (0.476235,0.8,0.16), (0.461176,0.8,0.16), (0.446118,0.8,0.16), (0.431059,0.8,0.16), (0.416,0.8,0.16), (0.400941,0.8,0.16), (0.385882,0.8,0.16), (0.370824,0.8,0.16), (0.355765,0.8,0.16), (0.340706,0.8,0.16), (0.325647,0.8,0.16), (0.310588,0.8,0.16), (0.295529,0.8,0.16), (0.280471,0.8,0.16), (0.265412,0.8,0.16), (0.250353,0.8,0.16), (0.235294,0.8,0.16), (0.220235,0.8,0.16), (0.205176,0.8,0.16), (0.190118,0.8,0.16), (0.175059,0.8,0.16), (0.16,0.8,0.16), (0.16,0.8,0.175059), (0.16,0.8,0.190118), (0.16,0.8,0.205176), (0.16,0.8,0.220235), (0.16,0.8,0.235294), (0.16,0.8,0.250353), (0.16,0.8,0.265412), (0.16,0.8,0.280471), (0.16,0.8,0.295529), (0.16,0.8,0.310588), (0.16,0.8,0.325647), (0.16,0.8,0.340706), (0.16,0.8,0.355765), (0.16,0.8,0.370824), (0.16,0.8,0.385882), (0.16,0.8,0.400941), (0.16,0.8,0.416), (0.16,0.8,0.431059), (0.16,0.8,0.446118), (0.16,0.8,0.461176), (0.16,0.8,0.476235), (0.16,0.8,0.491294), (0.16,0.8,0.506353), (0.16,0.8,0.521412), (0.16,0.8,0.53647), (0.16,0.8,0.551529), (0.16,0.8,0.566588), (0.16,0.8,0.581647), (0.16,0.8,0.596706), (0.16,0.8,0.611765), (0.16,0.8,0.626823), (0.16,0.8,0.641882), (0.16,0.8,0.656941), (0.16,0.8,0.672), (0.16,0.8,0.687059), (0.16,0.8,0.702118), (0.16,0.8,0.717176), (0.16,0.8,0.732235), (0.16,0.8,0.747294), (0.16,0.8,0.762353), (0.16,0.8,0.777412), (0.16,0.8,0.792471), (0.16,0.792471,0.8), (0.16,0.777412,0.8), (0.16,0.762353,0.8), (0.16,0.747294,0.8), (0.16,0.732235,0.8), (0.16,0.717176,0.8), (0.16,0.702118,0.8), (0.16,0.687059,0.8), (0.16,0.672,0.8), (0.16,0.656941,0.8), (0.16,0.641882,0.8), (0.16,0.626823,0.8), (0.16,0.611765,0.8), (0.16,0.596706,0.8), (0.16,0.581647,0.8), (0.16,0.566588,0.8), (0.16,0.551529,0.8), (0.16,0.53647,0.8), (0.16,0.521412,0.8), (0.16,0.506353,0.8), (0.16,0.491294,0.8), (0.16,0.476235,0.8), (0.16,0.461176,0.8), (0.16,0.446118,0.8), (0.16,0.431059,0.8), (0.16,0.416,0.8), (0.16,0.400941,0.8), (0.16,0.385882,0.8), (0.16,0.370824,0.8), (0.16,0.355765,0.8), (0.16,0.340706,0.8), (0.16,0.325647,0.8), (0.16,0.310588,0.8), (0.16,0.295529,0.8), (0.16,0.280471,0.8), (0.16,0.265412,0.8), (0.16,0.250353,0.8), (0.16,0.235294,0.8), (0.16,0.220235,0.8), (0.16,0.205176,0.8), (0.16,0.190118,0.8), (0.16,0.175059,0.8), (0.16,0.16,0.8), (0.175059,0.16,0.8), (0.190117,0.16,0.8), (0.205176,0.16,0.8), (0.220235,0.16,0.8), (0.235294,0.16,0.8), (0.250353,0.16,0.8), (0.265412,0.16,0.8), (0.280471,0.16,0.8), (0.295529,0.16,0.8), (0.310588,0.16,0.8), (0.325647,0.16,0.8), (0.340706,0.16,0.8), (0.355765,0.16,0.8), (0.370823,0.16,0.8), (0.385882,0.16,0.8), (0.400941,0.16,0.8), (0.416,0.16,0.8), (0.431059,0.16,0.8), (0.446118,0.16,0.8), (0.461176,0.16,0.8), (0.476235,0.16,0.8), (0.491294,0.16,0.8), (0.506353,0.16,0.8), (0.521412,0.16,0.8), (0.536471,0.16,0.8), (0.551529,0.16,0.8), (0.566588,0.16,0.8), (0.581647,0.16,0.8), (0.596706,0.16,0.8), (0.611765,0.16,0.8), (0.626823,0.16,0.8), (0.641882,0.16,0.8), (0.656941,0.16,0.8), (0.672,0.16,0.8), (0.687059,0.16,0.8), (0.702118,0.16,0.8), (0.717176,0.16,0.8), (0.732235,0.16,0.8), (0.747294,0.16,0.8), (0.762353,0.16,0.8), (0.777412,0.16,0.8), (0.79247,0.16,0.8), (0.8,0.16,0.792471), (0.8,0.16,0.777412), (0.8,0.16,0.762353), (0.8,0.16,0.747294), (0.8,0.16,0.732235), (0.8,0.16,0.717176), (0.8,0.16,0.702118), (0.8,0.16,0.687059), (0.8,0.16,0.672), (0.8,0.16,0.656941), (0.8,0.16,0.641882), (0.8,0.16,0.626824), (0.8,0.16,0.611765), (0.8,0.16,0.596706), (0.8,0.16,0.581647), (0.8,0.16,0.566588), (0.8,0.16,0.55153), (0.8,0.16,0.536471), (0.8,0.16,0.521412), (0.8,0.16,0.506353), (0.8,0.16,0.491294), (0.8,0.16,0.476235), (0.8,0.16,0.461176), (0.8,0.16,0.446118), (0.8,0.16,0.431059), (0.8,0.16,0.416), (0.8,0.16,0.400941), (0.8,0.16,0.385883), (0.8,0.16,0.370824), (0.8,0.16,0.355765), (0.8,0.16,0.340706), (0.8,0.16,0.325647), (0.8,0.16,0.310588), (0.8,0.16,0.295529), (0.8,0.16,0.280471), (0.8,0.16,0.265412), (0.8,0.16,0.250353), (0.8,0.16,0.235294), (0.8,0.16,0.220235), (0.8,0.16,0.205177), (0.8,0.16,0.190118), (0.8,0.16,0.175059)]

def plot_color_graph_from_file(path: str):
    x_plot, p1, p2, p3, rep = [], [], [], [], []
    alpha = 0.2
    with open(path, "r") as file:
        reader = csv.reader(file, delimiter=" ")
        line_len = None
        for line in reader:
            assert line_len is None or line_len == len(line)
            line_len = len(line)
            assert line_len in [2,3, 4, 5]

            x_plot.append(float(line[0]))
            if line_len == 4:
                p1.append(float(line[1]))
                p2.append(float(line[2]))
                p3.append(float(line[3]))
            elif line_len == 5:
                p1.append(float(line[1]))
                p2.append(float(line[2]))
                p3.append(float(line[3]))
                rep.append(float(line[4]))
            elif line_len == 3:
                p1.append(float(line[1]))
                rep.append(float(line[2]))
            else:
                p1.append(float(line[1]))
    
    #assert len(x_plot) == 256
    fig = plt.figure(num=1, clear=True)
    ax = fig.add_subplot()

    x_plot = np.array(x_plot)


    ax.set_xlabel('valeur pixel')
    ax.set_ylabel('nb occurences')
    if line_len >= 4:
        red_plot = np.array(p1)
        green_plot = np.array(p2)
        blue_plot = np.array(p3)
        repartition = np.array(rep)
        

        # Graphes en fonctions
        
        ax.stackplot(x_plot, red_plot, color = (1, 0, 0, alpha))
        ax.plot(x_plot, red_plot, color=(1, 0, 0))

        ax.stackplot(x_plot, green_plot, color = (0, 1, 0, alpha))
        ax.plot(x_plot, green_plot, color=(0, 1, 0))

        ax.stackplot(x_plot, blue_plot, color = (0, 0, 1, alpha))
        ax.plot(x_plot, blue_plot, color=(0, 0, 1))

        if line_len == 5:
            ax.set_ylabel('proba occurence')
            ax2 = ax.twinx()
            ax2.set_ylabel('répartition')
            ax2.plot(x_plot, repartition, color = (0.1, 0.1, 0.1))
            

    
    else:
        grey_plot = np.array(p1)
        repartition = np.array(rep)

        ax.stackplot(x_plot, grey_plot, color = (.5, .5, .5, alpha))
        ax.plot(x_plot, grey_plot, color=(.5, .5, .5))
        if line_len == 3:
            ax.set_ylabel('proba occurence')
            ax2 = ax.twinx()
            ax2.set_ylabel('répartition')
            ax2.plot(x_plot, repartition, color = (0.1, 0.1, 0.1))

    # Setup x axis
    #ax.set_xlim([x_plot[0], len(x_plot)])
    nb_values = 10
    #ax.set_xticks(np.linspace(0, 255, nb_values).astype('int'), labels=np.linspace(0, 255, nb_values).astype('int').astype('str'))

    ax.set_title(
        "Histogramme de " + os.path.basename(path)
    )

    return fig
    #plt.show()

def hue_plot(path):

    x_plot, p1 = [], []
    alpha = 0.2
    with open(path, "r") as file:
        reader = csv.reader(file, delimiter=" ")
        line_len = None
        for line in reader:
            assert line_len is None or line_len == len(line)
            line_len = len(line)
            assert line_len in [2]

            x_plot.append(float(line[0]))
            p1.append(float(line[1]))
    
    #assert len(x_plot) == 256
    fig = plt.figure(num=1, clear=True)
    ax = fig.add_subplot()

    x_plot = np.array(x_plot)
    
    fig = plt.figure(num=1, clear=True)
    ax = fig.add_subplot()

    # Compute pie slices
    N = 256
    theta = np.linspace(0.0, 2 * np.pi, N, endpoint=False)
    radii = 0.5
    width = 2*np.pi / 256 + 0.01

    ax = plt.subplot(projection='polar')
    ax.bar(theta, radii, width=width, bottom=0.5, color=colors, alpha=1.0)

    return fig


directory_in_str = "./img_out/"

directory = os.fsencode(directory_in_str)
if not os.path.exists(directory_in_str + "export_png/" ):
    os.makedirs(directory_in_str + "export_png/" )

if not os.path.exists(directory_in_str + "export_hist/" ):
    os.makedirs(directory_in_str + "export_hist/" )

fig = hue_plot()
fig.savefig(directory_in_str + "export_hist/" + "hello" + "_HUE.png")

n_pngs = 0
n_histos = 0
for file in os.listdir(directory):
    filename = os.fsdecode(file)
    if filename.endswith(".ppm"): 
        im = Image.open(directory_in_str + filename)
        im.save(directory_in_str + "export_png/" + filename[0:-4] + ".png")
        n_pngs += 1
    elif filename.endswith(".pgm"): 
        im = Image.open(directory_in_str + filename)
        im.save(directory_in_str + "export_png/" + filename[0:-4] + ".png")
        n_pngs += 1
    elif filename.endswith(".dat"):
        fig = plot_color_graph_from_file(directory_in_str + filename)
        fig.savefig(directory_in_str + "export_hist/" + filename[0:-4] + "_HISTOGRAM.png")
        n_histos += 1
    
    elif filename.endswith(".Hdat"):
        fig = hue_plot(directory_in_str + filename)
        fig.savefig(directory_in_str + "export_hist/" + filename[0:-5] + "_HUE.png")
        n_histos += 1


print("Done!\nCreated ", n_pngs, " images and ", n_histos, " histograms.")
