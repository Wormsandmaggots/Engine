#ifndef ENGINE_LINERENDERER_H
#define ENGINE_LINERENDERER_H

#include "glm/vec3.hpp"
#include "glad/glad.h"

class LineRenderer {
public:
   /*void drawLine(const glm::vec3& start, const glm::vec3& end) {
        glBegin(GL_LINES);
        glVertex3f(start.x, start.y, start.z);
        glVertex3f(end.x, end.y, end.z);
        glEnd();
    }*/
  void drawLine(const glm::vec3& start, const glm::vec3& end) {
       glLineWidth(800.0f); // Ustaw grubość linii na 2.0
       glColor3f(1.0f, 0.0f, 0.0f); // Ustaw kolor rysowania na czerwony
       glBegin(GL_LINES);
       glVertex3f(start.x, start.y, start.z);
       glVertex3f(end.x, end.y, end.z);
       glEnd();
   }


};

#endif