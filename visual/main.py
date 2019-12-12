import sys
import numpy as np
from pyqtgraph.Qt import QtGui, QtCore
import pyqtgraph as pg
import pyqtgraph.exporters


width = 800
height = 600

class Visualizer:
    def __init__(self, data, ranges, title, export):
        app = QtGui.QApplication([])
        win = pg.GraphicsWindow(title="window")
        win.resize(width, height)
        win.setWindowTitle("Collision animation")
        self.win = win
        self.export = export
        pg.setConfigOptions(antialias=True)
        self.plt = win.addPlot(title=title)
        self.plt.setXRange(min=ranges["xmin"], max=ranges["xmax"])
        self.plt.setYRange(min=ranges["ymin"], max=ranges["ymax"])
        self.plt.enableAutoRange("xy", False)
        self.plt.setAspectLocked(lock=True, ratio=1)
        self.plt.showAxis("bottom", True)
        self.plt.showAxis("left", True)
        self.scatter = self.plt.plot(
                [],
                [],
                pen=None,
                symbol="o",
                symbolPen=None,
                symbolSize=1.0,
                pxMode=False,
                symbolBrush=(255,0,0,255)
        )
        timer = QtCore.QTimer()
        self.i = 0
        self.data = data
        timer.timeout.connect(self.update)
        timer.start(50)
        if (sys.flags.interactive != 1) or not hasattr(QtCore, "PYQT_VERSION"):
            QtGui.QApplication.instance().exec_()

    def update(self):
        xs = self.data[self.i][0::2]
        ys = self.data[self.i][1::2]
        self.scatter.setData(xs, ys)
        pg.QtGui.QApplication.processEvents()
        if self.export:
            exporter = pg.exporters.ImageExporter(self.win.scene())
            exporter.export("png/img{:05d}.png".format(self.i))
        self.i += 1
        if self.i >= len(self.data):
            sys.exit()


if __name__ == "__main__":
    ranges = {"xmin": 0., "xmax": 8., "ymin": 0., "ymax": 6.}
    data = np.loadtxt("../positions.dat", dtype=np.float32)
    visualizer = Visualizer(
            data = data,
            ranges = ranges,
            title = "test",
            export=False
    )

