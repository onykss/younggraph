import svgwrite
#import sys
from svglib.svglib import svg2rlg
from reportlab.graphics import renderPDF
import re

edges_necessary = True
color_necessary = True

def visualize(parts, edges, str_parts, clrs):
    ver_dict = []

    colors = ['#FFFFFF', '#FDB4B4', '#FF7D7D', '#FF4343', '#FF0000']

    for b in str_parts:
        for e in b:
            ver_dict.append(e)

    #словарь с координатами для входа и выхода ребер
    ver_coords = {}

    #начальные координаты. Заменить на максимальный ряд
    wigth = 0
    max_wigth = 0
    num = 0

    for b in range(0, len(parts)):
        for e in parts[b]:
            wigth += len(e)
        if wigth > max_wigth:
            max_wigth = wigth
            wigth = 0
            num = b

    wigth = max_wigth
    wigth = wigth + 2 * (len(parts[num]) - 1) + 5
    height = 2
    for b in range(1, len(parts) + 1):
        height += b + 2
    height *= 10
    if wigth % 2 == 0:
        wigth += 1
    x = wigth // 2
    wigth *= 10
    x *= 10
    y = 10
    start_x = x
    start_y = y
    ex_wigth = 1

    graph = svgwrite.Drawing('svgimg.svg', size=(str(wigth) + "px", str(height) + "px"), profile='full')

    for b in range(0, len(parts)):
        if b > 0:
            wigth = 0
            for e in parts[b]:
                wigth += len(e)
            wigth += (2 * (len(parts[b]) - 1))
            offset = (wigth - ex_wigth) // 2
            ex_wigth = wigth
            start_x -= offset * 10
            x = start_x

        for e in range(0, len(parts[b])):
            y += (b + 1 - int(parts[b][e][0])) * 10

            fy = y
            fx = x
            fstart_y = fy
            for k in range(0, len(parts[b][e])):
                for g in range(0, int(parts[b][e][k])):

                    if color_necessary:
                        graph.add(graph.rect((fx, fy + (10 * g)), (10, 10), fill='#FF0000', fill_opacity=clrs[b][e], stroke='black'))
                    else:
                        graph.add(graph.rect((fx, fy + (10 * g)), (10, 10), fill='#FFFFFF', stroke='black'))

                fx += 10
                if k < len(parts[b][e]) - 1:
                    fstart_y = fstart_y + 10 * (int(parts[b][e][k]) - int(parts[b][e][k + 1]))
                    fy = fstart_y

            x_in = x + 5 * len(parts[b][e])
            y_in = y - 2
            x_out = x + 5 * len(parts[b][e])
            y_out = y + 10 * int(parts[b][e][0]) + 2
            ver_coords[str_parts[b][e]] = [x_in, y_in, x_out, y_out]

            x += (len(parts[b][e]) + 2) * 10
            y = start_y
            if b == 1:
                x += 10
        start_y += (int(parts[b][0][0]) + 2) * 10
        y = start_y

    if edges_necessary:
        for b in range(0, len(edges)):
            for e in range(0, len(edges[b])):
                if int(edges[b][e]) == 1:
                    graph.add(graph.line((ver_coords[ver_dict[b]][2], ver_coords[ver_dict[b]][3]),
                                         (ver_coords[ver_dict[e]][0], ver_coords[ver_dict[e]][1]), stroke='black'))

    graph.save()

        #импорт в пдф

    drawing = svg2rlg("svgimg.svg")
    renderPDF.drawToFile(drawing, "graph.pdf")


if __name__ == "__main__":
    data = open('vizualize\data.txt', 'r')
    args = data.read()
    data.close()
    args = args.split(" ")

    parts = []
    str_parts = []
    part = []
    str_part = []
    flag = 0
    last_sum = 1
    summ = 0
    edges = []
    Gclrs = []
    Sclrs = []


    for i in args:
        i = re.sub("'", "", i)

        if i == '--args1':
            flag = 1

        elif flag == 1 and i != '--args2':
            el = i
            x = i.split("$")
            i = x[0]
            c = float(x[1])
            i = i.split(";")
            for j in i:
                summ += int(j)

            if summ == last_sum:
                part.append(i)
                Sclrs.append(c)
                str_part.append(el)
            else:
                parts.append(part)
                Gclrs.append(Sclrs)
                part = [i]
                Sclrs = [c]
                last_sum = summ

                str_parts.append(str_part)
                str_part = [el]
            summ = 0

        elif i == '--args2':
            flag = 2
            parts.append(part)
            Gclrs.append(Sclrs)
            str_parts.append(str_part)

        elif i == '--args0':
            flag = 3
        elif flag == 3:
            if int(i[0]) == 1:
                edges_necessary = True
            else:
                edges_necessary = False
            if int(i[1]) == 1:
                color_necessary = True
            else:
                color_necessary = False
            flag = 0
        elif flag == 2:
            edges.append(i)

    visualize(parts, edges, str_parts, Gclrs)