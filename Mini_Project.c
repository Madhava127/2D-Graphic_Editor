/*
 * 2D Graphics Editor
 * Uses a 2D character array as canvas
 * Draws: Circle, Rectangle, Line, Triangle
 * Supports: Add, Delete, Modify objects
 *
 * Author: Madhava
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ─── Canvas settings ─────────────────────────────────────────── */
#define ROWS   25
#define COLS   60
#define MAX_OBJ 20

#define FILL_CHAR  '*'
#define EDGE_CHAR  '_'
#define BLANK      ' '

/* ─── Object types ────────────────────────────────────────────── */
#define OBJ_NONE      0
#define OBJ_CIRCLE    1
#define OBJ_RECT      2
#define OBJ_LINE      3
#define OBJ_TRIANGLE  4

/* ─── Data structures ─────────────────────────────────────────── */
typedef struct {
    int type;          /* OBJ_* constant          */
    int x1, y1;        /* primary point / center  */
    int x2, y2;        /* secondary point / size  */
    int x3, y3;        /* third point (triangle)  */
    int radius;        /* circle radius            */
    char symbol;       /* '*' or '_'               */
} Object;

/* Global state */
char    canvas[ROWS][COLS];
Object  objects[MAX_OBJ];
int     obj_count = 0;

/* ─── Canvas helpers ──────────────────────────────────────────── */
void clear_canvas(void) {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            canvas[r][c] = BLANK;
}

/* Safe pixel setter */
void set_pixel(int r, int c, char ch) {
    if (r >= 0 && r < ROWS && c >= 0 && c < COLS)
        canvas[r][c] = ch;
}

void display_picture(void) {
    /* Top border */
    printf("+");
    for (int c = 0; c < COLS; c++) printf("-");
    printf("+\n");

    for (int r = 0; r < ROWS; r++) {
        printf("|");
        for (int c = 0; c < COLS; c++)
            putchar(canvas[r][c]);
        printf("|\n");
    }

    /* Bottom border */
    printf("+");
    for (int c = 0; c < COLS; c++) printf("-");
    printf("+\n");
}

/* ─── Drawing functions ───────────────────────────────────────── */

/* Bresenham line */
void draw_line(int r1, int c1, int r2, int c2, char ch) {
    int dr = abs(r2 - r1), dc = abs(c2 - c1);
    int sr = (r1 < r2) ? 1 : -1;
    int sc = (c1 < c2) ? 1 : -1;
    int err = dr - dc;

    while (1) {
        set_pixel(r1, c1, ch);
        if (r1 == r2 && c1 == c2) break;
        int e2 = 2 * err;
        if (e2 > -dc) { err -= dc; r1 += sr; }
        if (e2 <  dr) { err += dr; c1 += sc; }
    }
}

/* Rectangle: (r1,c1) top-left, (r2,c2) bottom-right */
void draw_rectangle(int r1, int c1, int r2, int c2, char ch) {
    draw_line(r1, c1, r1, c2, ch);   /* top    */
    draw_line(r2, c1, r2, c2, ch);   /* bottom */
    draw_line(r1, c1, r2, c1, ch);   /* left   */
    draw_line(r1, c2, r2, c2, ch);   /* right  */
}

/* Circle using midpoint algorithm */
void draw_circle(int cr, int cc, int radius, char ch) {
    int x = radius, y = 0, err = 0;
    while (x >= y) {
        set_pixel(cr + y, cc + x, ch);
        set_pixel(cr + y, cc - x, ch);
        set_pixel(cr - y, cc + x, ch);
        set_pixel(cr - y, cc - x, ch);
        set_pixel(cr + x, cc + y, ch);
        set_pixel(cr + x, cc - y, ch);
        set_pixel(cr - x, cc + y, ch);
        set_pixel(cr - x, cc - y, ch);
        y++;
        err += 2 * y + 1;
        if (2 * (err - x) + 1 > 0) { x--; err += 1 - 2 * x; }
    }
}

/* Triangle: three vertices */
void draw_triangle(int r1, int c1, int r2, int c2, int r3, int c3, char ch) {
    draw_line(r1, c1, r2, c2, ch);
    draw_line(r2, c2, r3, c3, ch);
    draw_line(r3, c3, r1, c1, ch);
}

/* ─── Redraw all objects onto a blank canvas ──────────────────── */
void redraw_all(void) {
    clear_canvas();
    for (int i = 0; i < obj_count; i++) {
        Object *o = &objects[i];
        if (o->type == OBJ_NONE) continue;
        switch (o->type) {
            case OBJ_CIRCLE:
                draw_circle(o->x1, o->y1, o->radius, o->symbol); break;
            case OBJ_RECT:
                draw_rectangle(o->x1, o->y1, o->x2, o->y2, o->symbol); break;
            case OBJ_LINE:
                draw_line(o->x1, o->y1, o->x2, o->y2, o->symbol); break;
            case OBJ_TRIANGLE:
                draw_triangle(o->x1, o->y1, o->x2, o->y2,
                              o->x3, o->y3, o->symbol); break;
        }
    }
}

/* ─── Object management ───────────────────────────────────────── */
void list_objects(void) {
    if (obj_count == 0) { printf("  (no objects yet)\n"); return; }
    for (int i = 0; i < obj_count; i++) {
        Object *o = &objects[i];
        if (o->type == OBJ_NONE) continue;
        printf("  [%d] ", i);
        switch (o->type) {
            case OBJ_CIRCLE:
                printf("Circle   centre(%d,%d) r=%d sym='%c'\n",
                       o->x1, o->y1, o->radius, o->symbol); break;
            case OBJ_RECT:
                printf("Rect     (%d,%d)->(%d,%d) sym='%c'\n",
                       o->x1, o->y1, o->x2, o->y2, o->symbol); break;
            case OBJ_LINE:
                printf("Line     (%d,%d)->(%d,%d) sym='%c'\n",
                       o->x1, o->y1, o->x2, o->y2, o->symbol); break;
            case OBJ_TRIANGLE:
                printf("Triangle (%d,%d) (%d,%d) (%d,%d) sym='%c'\n",
                       o->x1, o->y1, o->x2, o->y2,
                       o->x3, o->y3, o->symbol); break;
        }
    }
}

/* Add helpers */
void add_circle(void) {
    if (obj_count >= MAX_OBJ) { printf("Object limit reached.\n"); return; }
    Object *o = &objects[obj_count];
    o->type = OBJ_CIRCLE;
    o->symbol = FILL_CHAR;
    printf("  Centre row col (e.g. 12 30): ");
    scanf("%d %d", &o->x1, &o->y1);
    printf("  Radius: ");
    scanf("%d", &o->radius);
    obj_count++;
}

void add_rectangle(void) {
    if (obj_count >= MAX_OBJ) { printf("Object limit reached.\n"); return; }
    Object *o = &objects[obj_count];
    o->type = OBJ_RECT;
    o->symbol = EDGE_CHAR;
    printf("  Top-left row col: ");
    scanf("%d %d", &o->x1, &o->y1);
    printf("  Bottom-right row col: ");
    scanf("%d %d", &o->x2, &o->y2);
    obj_count++;
}

void add_line(void) {
    if (obj_count >= MAX_OBJ) { printf("Object limit reached.\n"); return; }
    Object *o = &objects[obj_count];
    o->type = OBJ_LINE;
    o->symbol = FILL_CHAR;
    printf("  Start row col: ");
    scanf("%d %d", &o->x1, &o->y1);
    printf("  End row col: ");
    scanf("%d %d", &o->x2, &o->y2);
    obj_count++;
}

void add_triangle(void) {
    if (obj_count >= MAX_OBJ) { printf("Object limit reached.\n"); return; }
    Object *o = &objects[obj_count];
    o->type = OBJ_TRIANGLE;
    o->symbol = FILL_CHAR;
    printf("  Vertex 1 row col: ");
    scanf("%d %d", &o->x1, &o->y1);
    printf("  Vertex 2 row col: ");
    scanf("%d %d", &o->x2, &o->y2);
    printf("  Vertex 3 row col: ");
    scanf("%d %d", &o->x3, &o->y3);
    obj_count++;
}

void delete_object(void) {
    list_objects();
    if (obj_count == 0) return;
    int idx;
    printf("  Enter object number to delete: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= obj_count || objects[idx].type == OBJ_NONE) {
        printf("  Invalid index.\n"); return;
    }
    /* Shift remaining objects left */
    for (int i = idx; i < obj_count - 1; i++)
        objects[i] = objects[i + 1];
    obj_count--;
    printf("  Object deleted.\n");
}

void modify_object(void) {
    list_objects();
    if (obj_count == 0) return;
    int idx;
    printf("  Enter object number to modify: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= obj_count || objects[idx].type == OBJ_NONE) {
        printf("  Invalid index.\n"); return;
    }
    /* Re-enter same type with new values */
    Object *o = &objects[idx];
    printf("  Enter new values (same shape type kept):\n");
    switch (o->type) {
        case OBJ_CIRCLE:
            printf("  New centre row col: ");
            scanf("%d %d", &o->x1, &o->y1);
            printf("  New radius: ");
            scanf("%d", &o->radius);
            break;
        case OBJ_RECT:
            printf("  New top-left row col: ");
            scanf("%d %d", &o->x1, &o->y1);
            printf("  New bottom-right row col: ");
            scanf("%d %d", &o->x2, &o->y2);
            break;
        case OBJ_LINE:
            printf("  New start row col: ");
            scanf("%d %d", &o->x1, &o->y1);
            printf("  New end row col: ");
            scanf("%d %d", &o->x2, &o->y2);
            break;
        case OBJ_TRIANGLE:
            printf("  New vertex 1 row col: ");
            scanf("%d %d", &o->x1, &o->y1);
            printf("  New vertex 2 row col: ");
            scanf("%d %d", &o->x2, &o->y2);
            printf("  New vertex 3 row col: ");
            scanf("%d %d", &o->x3, &o->y3);
            break;
    }
    printf("  Object modified.\n");
}

/* ─── Menus ───────────────────────────────────────────────────── */
void add_menu(void) {
    printf("\n  ADD OBJECT\n");
    printf("  1. Circle\n");
    printf("  2. Rectangle\n");
    printf("  3. Line\n");
    printf("  4. Triangle\n");
    printf("  Choice: ");
    int ch; scanf("%d", &ch);
    switch (ch) {
        case 1: add_circle();    break;
        case 2: add_rectangle(); break;
        case 3: add_line();      break;
        case 4: add_triangle();  break;
        default: printf("  Invalid choice.\n");
    }
}

void main_menu(void) {
    int choice;
    do {
        redraw_all();
        display_picture();

        printf("\n========== 2D GRAPHICS EDITOR ==========\n");
        printf(" 1. Add object\n");
        printf(" 2. Delete object\n");
        printf(" 3. Modify object\n");
        printf(" 4. List objects\n");
        printf(" 5. Clear canvas\n");
        printf(" 0. Exit\n");
        printf("=========================================\n");
        printf(" Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_menu();       break;
            case 2: delete_object();  break;
            case 3: modify_object();  break;
            case 4: list_objects();   break;
            case 5:
                obj_count = 0;
                clear_canvas();
                printf("  Canvas cleared.\n");
                break;
            case 0: printf("  Goodbye!\n"); break;
            default: printf("  Invalid option.\n");
        }
    } while (choice != 0);
}

/* ─── Entry point ─────────────────────────────────────────────── */
int main(void) {
    clear_canvas();
    main_menu();
    return 0;
}
