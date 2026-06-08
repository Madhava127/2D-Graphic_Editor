#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 30
#define COLS 80
#define MAX_OBJECTS 100

char canvas[ROWS][COLS];

typedef enum {
    RECTANGLE = 1,
    LINE,
    CIRCLE,
    TRIANGLE
} ShapeType;

typedef struct {
    int id;
    ShapeType type;
    int active;

    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;
int nextId = 1;

/* ---------- Canvas Functions ---------- */

void initCanvas() {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            canvas[i][j] = '_';
        }
    }
}

void displayCanvas() {
    printf("\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

/* ---------- Utility ---------- */

void plot(int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS)
        canvas[y][x] = '*';
}

/* ---------- Drawing Functions ---------- */

void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while (1) {
        plot(x1, y1);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x, int y, int width, int height) {

    for (int i = x; i < x + width; i++) {
        plot(i, y);
        plot(i, y + height - 1);
    }

    for (int i = y; i < y + height; i++) {
        plot(x, i);
        plot(x + width - 1, i);
    }
}

void plotCirclePoints(int xc, int yc, int x, int y) {
    plot(xc + x, yc + y);
    plot(xc - x, yc + y);
    plot(xc + x, yc - y);
    plot(xc - x, yc - y);

    plot(xc + y, yc + x);
    plot(xc - y, yc + x);
    plot(xc + y, yc - x);
    plot(xc - y, yc - x);
}

void drawCircle(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int d = 1 - r;

    while (x <= y) {
        plotCirclePoints(xc, yc, x, y);

        if (d < 0)
            d += 2 * x + 3;
        else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void drawTriangle(int x1, int y1,
                  int x2, int y2,
                  int x3, int y3) {

    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

/* ---------- Redraw Entire Scene ---------- */

void redrawAll() {

    initCanvas();

    for (int i = 0; i < objectCount; i++) {

        if (!objects[i].active)
            continue;

        switch (objects[i].type) {

            case RECTANGLE:
                drawRectangle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2
                );
                break;

            case LINE:
                drawLine(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2
                );
                break;

            case CIRCLE:
                drawCircle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].radius
                );
                break;

            case TRIANGLE:
                drawTriangle(
                    objects[i].x1,
                    objects[i].y1,
                    objects[i].x2,
                    objects[i].y2,
                    objects[i].x3,
                    objects[i].y3
                );
                break;
        }
    }
}

/* ---------- Add Objects ---------- */

void addRectangle() {
    Object obj;

    obj.id = nextId++;
    obj.type = RECTANGLE;
    obj.active = 1;

    printf("Enter x y width height: ");
    scanf("%d %d %d %d",
          &obj.x1,
          &obj.y1,
          &obj.x2,
          &obj.y2);

    objects[objectCount++] = obj;
    redrawAll();

    printf("Rectangle added. ID = %d\n", obj.id);
}

void addLine() {
    Object obj;

    obj.id = nextId++;
    obj.type = LINE;
    obj.active = 1;

    printf("Enter x1 y1 x2 y2: ");
    scanf("%d %d %d %d",
          &obj.x1,
          &obj.y1,
          &obj.x2,
          &obj.y2);

    objects[objectCount++] = obj;
    redrawAll();

    printf("Line added. ID = %d\n", obj.id);
}

void addCircle() {
    Object obj;

    obj.id = nextId++;
    obj.type = CIRCLE;
    obj.active = 1;

    printf("Enter center_x center_y radius: ");
    scanf("%d %d %d",
          &obj.x1,
          &obj.y1,
          &obj.radius);

    objects[objectCount++] = obj;
    redrawAll();

    printf("Circle added. ID = %d\n", obj.id);
}

void addTriangle() {
    Object obj;

    obj.id = nextId++;
    obj.type = TRIANGLE;
    obj.active = 1;

    printf("Enter x1 y1 x2 y2 x3 y3: ");
    scanf("%d %d %d %d %d %d",
          &obj.x1,
          &obj.y1,
          &obj.x2,
          &obj.y2,
          &obj.x3,
          &obj.y3);

    objects[objectCount++] = obj;
    redrawAll();

    printf("Triangle added. ID = %d\n", obj.id);
}

/* ---------- Delete ---------- */

void deleteObject() {
    int id;

    printf("Enter object ID to delete: ");
    scanf("%d", &id);

    for (int i = 0; i < objectCount; i++) {
        if (objects[i].id == id && objects[i].active) {
            objects[i].active = 0;
            redrawAll();
            printf("Object deleted.\n");
            return;
        }
    }

    printf("Object not found.\n");
}

/* ---------- Modify ---------- */

void modifyObject() {

    int id;

    printf("Enter object ID to modify: ");
    scanf("%d", &id);

    for (int i = 0; i < objectCount; i++) {

        if (objects[i].id == id && objects[i].active) {

            switch (objects[i].type) {

                case RECTANGLE:
                    printf("Enter new x y width height: ");
                    scanf("%d %d %d %d",
                          &objects[i].x1,
                          &objects[i].y1,
                          &objects[i].x2,
                          &objects[i].y2);
                    break;

                case LINE:
                    printf("Enter new x1 y1 x2 y2: ");
                    scanf("%d %d %d %d",
                          &objects[i].x1,
                          &objects[i].y1,
                          &objects[i].x2,
                          &objects[i].y2);
                    break;

                case CIRCLE:
                    printf("Enter new center_x center_y radius: ");
                    scanf("%d %d %d",
                          &objects[i].x1,
                          &objects[i].y1,
                          &objects[i].radius);
                    break;

                case TRIANGLE:
                    printf("Enter new x1 y1 x2 y2 x3 y3: ");
                    scanf("%d %d %d %d %d %d",
                          &objects[i].x1,
                          &objects[i].y1,
                          &objects[i].x2,
                          &objects[i].y2,
                          &objects[i].x3,
                          &objects[i].y3);
                    break;
            }

            redrawAll();
            printf("Object modified.\n");
            return;
        }
    }

    printf("Object not found.\n");
}

/* ---------- List Objects ---------- */

void listObjects() {

    printf("\nCurrent Objects:\n");

    for (int i = 0; i < objectCount; i++) {

        if (objects[i].active) {

            printf("ID: %d  Type: ", objects[i].id);

            switch (objects[i].type) {
                case RECTANGLE:
                    printf("Rectangle");
                    break;
                case LINE:
                    printf("Line");
                    break;
                case CIRCLE:
                    printf("Circle");
                    break;
                case TRIANGLE:
                    printf("Triangle");
                    break;
            }

            printf("\n");
        }
    }
}

/* ---------- Main ---------- */

int main() {

    initCanvas();

    int choice;

    while (1) {

        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Rectangle\n");
        printf("2. Add Line\n");
        printf("3. Add Circle\n");
        printf("4. Add Triangle\n");
        printf("5. Delete Object\n");
        printf("6. Modify Object\n");
        printf("7. List Objects\n");
        printf("8. Display Canvas\n");
        printf("9. Exit\n");
        printf("Enter choice: ");

        scanf("%d", &choice);

        switch (choice) {

            case 1:
                addRectangle();
                break;

            case 2:
                addLine();
                break;

            case 3:
                addCircle();
                break;

            case 4:
                addTriangle();
                break;

            case 5:
                deleteObject();
                break;

            case 6:
                modifyObject();
                break;

            case 7:
                listObjects();
                break;

            case 8:
                displayCanvas();
                break;

            case 9:
                printf("Exiting...\n");
                return 0;

            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}