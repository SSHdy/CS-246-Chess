/*
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <unistd.h>
#include "window.hpp"

Xwindow::Xwindow(int width, int height) : width(width), height(height) {
    d = XOpenDisplay(nullptr);
    if (d == nullptr) {
        std::cerr << "Cannot open display" << std::endl;
        exit(1);
    }
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                            BlackPixel(d, s), WhitePixel(d, s));
    XSelectInput(d, w, ExposureMask | KeyPressMask);
    XMapRaised(d, w);
    gc = XCreateGC(d, w, 0, nullptr);
    XSetForeground(d, gc, BlackPixel(d, s));

    const char *color_names[] = {"white", "black", "red", "green", "blue",
                                 "cyan", "yellow", "magenta", "orange", "brown"};
    Colormap cmap = DefaultColormap(d, DefaultScreen(d));
    for (int i = 0; i < 10; ++i) {
        XColor color;
        XParseColor(d, cmap, color_names[i], &color);
        XAllocColor(d, cmap, &color);
        colours[i] = color.pixel;
    }

    XSynchronize(d, True);
    usleep(1000);
}

Xwindow::~Xwindow() {
    XFreeGC(d, gc);
    XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
    XSetForeground(d, gc, colours[colour]);
    XFillRectangle(d, w, gc, x, y, width, height);
    XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawString(int x, int y, const std::string &msg, int colour) {
    XSetForeground(d, gc, colours[colour]);
    XDrawString(d, w, gc, x, y, msg.c_str(), msg.length());
}

void Xwindow::drawBigString(int x, int y, const std::string &msg, int colour) {
    drawString(x, y, msg, colour);
}
*/
