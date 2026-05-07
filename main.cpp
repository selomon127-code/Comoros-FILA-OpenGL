 // ============================================================
//  Comoros Flag & FILA Logo – OpenGL / GLUT (C++)
// ============================================================

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <cmath>

const int W = 900, H = 420;
const float SCALE = 0.88f;

float waveOffset = 0.12f;
// ─────────────────────────────────────────────────────────────
static void setColor(float r, float g, float b)
{
    glColor3f(r, g, b);
}

// ─────────────────────────────────────────────────────────────
static void fillRect(float x, float y, float w, float h)
{
    glBegin(GL_QUADS);

    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);

    glEnd();
}

// ─────────────────────────────────────────────────────────────
static void fillCircle(float cx, float cy, float r, int segs = 120)
{
    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(cx, cy);

    for(int i = 0; i <= segs; i++)
    {
        float a = 2.0f * M_PI * i / segs;

        glVertex2f(
            cx + r * cosf(a),
            cy + r * sinf(a)
        );
    }

    glEnd();
}

// ─────────────────────────────────────────────────────────────
static void fillPoly(float* pts, int n)
{
    glBegin(GL_POLYGON);

    for(int i = 0; i < n; i++)
    {
        glVertex2f(pts[2*i], pts[2*i+1]);
    }

    glEnd();
}

// ─────────────────────────────────────────────────────────────
static void drawStar5(float cx,float cy,float outer,float inner,float tilt)
{
    const int N = 5;

    float vx[10], vy[10];

    for(int i = 0; i < N; i++)
    {
        float ao = tilt + 2.0f * M_PI * i / N;
        float ai = ao + M_PI / N;

        vx[2*i]   = cx + outer * cosf(ao);
        vy[2*i]   = cy + outer * sinf(ao);

        vx[2*i+1] = cx + inner * cosf(ai);
        vy[2*i+1] = cy + inner * sinf(ai);
    }

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(cx, cy);

    for(int i = 0; i < 10; i++)
    {
        glVertex2f(vx[i], vy[i]);
    }

    glVertex2f(vx[0], vy[0]);

    glEnd();
}

// ─────────────────────────────────────────────────────────────
static void drawCrescent(float cx,float cy,float R,float cutOff,float cutR)
{
    glEnable(GL_STENCIL_TEST);

    glStencilFunc(GL_ALWAYS,1,0xFF);
    glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);

    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);

    fillCircle(cx,cy,R);

    glStencilFunc(GL_ALWAYS,0,0xFF);

    fillCircle(cx + cutOff,cy,cutR);

    glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

    glStencilFunc(GL_EQUAL,1,0xFF);
    glStencilOp(GL_KEEP,GL_KEEP,GL_KEEP);

    setColor(1,1,1);

    fillCircle(cx,cy,R);

    glDisable(GL_STENCIL_TEST);

    glClear(GL_STENCIL_BUFFER_BIT);
}

// ─────────────────────────────────────────────────────────────
static void drawFlag()
{
    const float FX = 30.0f * SCALE;
    const float FY = 70.0f * SCALE;

    const float FW = 440.0f * SCALE;
    const float FH = 280.0f * SCALE;

    // ── Flag Pole ───────────────────────────────────────────────
setColor(0.35f, 0.35f, 0.35f);

glLineWidth(8);

glBegin(GL_LINES);

glVertex2f(FX - 8.0f, FY - 20.0f);      // bottom
glVertex2f(FX - 8.0f, FY + FH + 20.0f); // top

glEnd();

    const float stripeH = FH / 4.0f;

    float sc[4][3] =
    {
        {0.00f,0.18f,0.68f},
        {0.82f,0.08f,0.08f},
        {0.96f,0.96f,0.96f},
        {0.96f,0.80f,0.00f}
    };

// ── Waving Flag Stripes ─────────────────────────────────────
// ── Waving Flag Stripes ─────────────────────────────────────
for(int s = 0; s < 4; s++)
{
    setColor(sc[s][0], sc[s][1], sc[s][2]);

    float y1 = FY + s * stripeH;
    float y2 = y1 + stripeH;

    glBegin(GL_QUAD_STRIP);

    for(int x = 0; x <= (int)FW; x += 8)
    {
        float wave =
            sinf((x * 0.03f) + waveOffset)
            * ((float)x / FW)
            * 10.0f;

        glVertex2f(FX + x, y1 + wave);
        glVertex2f(FX + x, y2 + wave);
    }

    glEnd();
}
    // Green Triangle
    setColor(0.00f,0.48f,0.13f);

    float triWidth = FW * 0.45f;

    float tri[6] =
    {
        FX, FY,
        FX + triWidth, FY + FH * 0.5f,
        FX, FY + FH
    };

    fillPoly(tri,3);

    // Moon
    float margin = 10.0f * SCALE;

    float moonR  = triWidth * 0.33f;

    float moonCX = FX + moonR + margin;
    float moonCY = FY + FH * 0.50f;

    float cutOff = moonR * 0.40f;
    float cutR   = moonR * 0.98f;

    drawCrescent(
        moonCX,
        moonCY,
        moonR,
        cutOff,
        cutR
    );

    // Stars
    float bandThick = moonR - (cutR - cutOff);

    float starOuter = bandThick * 0.44f;
    float starInner = starOuter * 0.35f;

    float starX = moonCX + moonR - starOuter * 3.5f;

    float tipClear = moonR * 0.25f;

    float vStep =
        (2.0f * (moonR - tipClear)) / 3.0f;

    float startY =
        moonCY - moonR + tipClear;

    setColor(1,1,1);

    for(int i = 0; i < 4; i++)
    {
        drawStar5(
            starX,
            startY + i * vStep,
            starOuter,
            starInner,
            -M_PI / 2.0f
        );
    }
}

// ─────────────────────────────────────────────────────────────
static void roundedRect2(float x,float y,float w,float h,float r)
{
    int seg = 24;

    glBegin(GL_POLYGON);

    for(int i=0;i<=seg;i++){
        float t = M_PI + (M_PI/2)*i/seg;
        glVertex2f(x+r+cos(t)*r,y+r+sin(t)*r);
    }

    for(int i=0;i<=seg;i++){
        float t = 1.5f*M_PI + (M_PI/2)*i/seg;
        glVertex2f(x+w-r+cos(t)*r,y+r+sin(t)*r);
    }

    for(int i=0;i<=seg;i++){
        float t = (M_PI/2)*i/seg;
        glVertex2f(x+w-r+cos(t)*r,y+h-r+sin(t)*r);
    }

    for(int i=0;i<=seg;i++){
        float t = M_PI/2 + (M_PI/2)*i/seg;
        glVertex2f(x+r+cos(t)*r,y+h-r+sin(t)*r);
    }

    glEnd();
}
void update(int value)
{
    // ONLY flag movement
    waveOffset += 0.08f;

    glutPostRedisplay();

    glutTimerFunc(16, update, 0);
}
// ─────────────────────────────────────────────────────────────
static void drawFILA()
{
    glPushMatrix();

    // Center position
glTranslatef(380.0f, 78.0f, 0);

// Bigger logo size
glScalef(220.0f,220.0f,1.0f);

    setColor(0,0,0);

    // ===== F =====
    roundedRect2(0.32f,0.50f,0.46f,0.18f,0.06f);
    roundedRect2(0.32f,0.30f,0.44f,0.18f,0.05f);
    roundedRect2(0.32f,0.05f,0.18f,0.40f,0.05f);

    // ===== I =====
    roundedRect2(0.80f,0.05f,0.16f,0.62f,0.06f);

    // ===== L =====
    roundedRect2(1.0f,0.05f,0.16f,0.62f,0.06f);
    roundedRect2(1.10f,0.05f,0.28f,0.15f,0.05f);

    // ===== A =====
    glBegin(GL_QUAD_STRIP);

    glVertex2f(1.28f,0.05f);
    glVertex2f(1.44f,0.05f);

    for(int i = 0; i <= 10; i++)
    {
        float angle = (float)i / 10.0f;

        float x_offset = 0.15f * angle;

        glVertex2f(
            1.28f + x_offset,
            0.05f + (0.51f * angle)
        );

        glVertex2f(
            1.44f + x_offset,
            0.05f + (0.51f * angle)
        );
    }

    glEnd();

    // Top connector
    glBegin(GL_POLYGON);

    glVertex2f(1.43f,0.56f);
    glVertex2f(1.77f,0.56f);
    glVertex2f(1.68f,0.65f);
    glVertex2f(1.52f,0.65f);

    glEnd();

    // Right leg
    glBegin(GL_POLYGON);

    glVertex2f(1.64f,0.56f);
    glVertex2f(1.77f,0.56f);
    glVertex2f(1.96f,0.05f);
    glVertex2f(1.80f,0.05f);

    glEnd();

    // Middle bar
    roundedRect2(1.50f,0.05f,0.38f,0.15f,0.05f);

    glPopMatrix();
}

// ─────────────────────────────────────────────────────────────
static void drawLabel()
{
    return;
}

// ─────────────────────────────────────────────────────────────
void display()
{
    glClearColor(1,1,1,1);

    glClear(
        GL_COLOR_BUFFER_BIT |
        GL_STENCIL_BUFFER_BIT
    );

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluOrtho2D(0,W,0,H);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();

    drawFlag();

    drawFILA();

    drawLabel();

    glutSwapBuffers();
}

// ─────────────────────────────────────────────────────────────
int main(int argc,char** argv)
{

    glutInit(&argc,argv);

    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGB |
        GLUT_STENCIL
    );

    glutInitWindowSize(W,H);

    glutInitWindowPosition(100,100);

    glutCreateWindow(
        "Comoros Flag & FILA Logo"
    );

    glutDisplayFunc(display);
    glutTimerFunc(16, update, 0);

    glutMainLoop();

    return 0;
}
