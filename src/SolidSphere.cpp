#include "SolidSphere.h"


SolidSphere::SolidSphere (float radius)
{
  this->r = radius;
  glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	for (int i = 0; i <= SPHERE_NUM_LAT; i++) {
    for (int j = 0; j <= SPHERE_NUM_LON; j++) {
      double lat = ((double)i) / SPHERE_NUM_LAT;
      double lon = ((double)j) / SPHERE_NUM_LON;
      GLfloat *vptr = &Vertices[3 * SINDEX(i, j)];
      GLfloat *nptr = &Normals[3 * SINDEX(i, j)];

      lat *= M_PI;
      lon *= 2 * M_PI;
      double sinlat = sin(lat);

      nptr[0] = vptr[0] = sinlat * sin(lon);
      nptr[1] = vptr[1] = cos(lat);
      nptr[2] = vptr[2] = sinlat * cos(lon);
    }
  }

  for (int i = 0; i < SPHERE_NUM_LAT; i++) {
    for (int j = 0; j < SPHERE_NUM_LON; j++) {
      unsigned int *iptr = &Indices[6 * (SPHERE_NUM_LON * i + j)];

      unsigned int i00 = SINDEX(i, j);
      unsigned int i10 = SINDEX(i + 1, j);
      unsigned int i11 = SINDEX(i + 1, j + 1);
      unsigned int i01 = SINDEX(i, j + 1);

      iptr[0] = i00;
      iptr[1] = i10;
      iptr[2] = i11;
      iptr[3] = i11;
      iptr[4] = i01;
      iptr[5] = i00;
    }
  }

	glGenBuffers(1, &vbo[VERTEX_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, 3 * SPHERE_NUM_INDICES * sizeof(GLfloat), Vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &vbo[NORMAL_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, 3 * SPHERE_NUM_INDICES * sizeof(GLfloat), Normals, GL_STATIC_DRAW);

	/**
	glGenBuffers(1, &vbo[TEXCOORD_BUFFER]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[TEXCOORD_BUFFER]);
	glBufferData(GL_ARRAY_BUFFER, 2 * faces * sizeof(GLfloat), tt, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (vao, 1);


	unsigned int *i = new unsigned int[faces * 4];
	for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
    *i++ = r * sectors + s;
    *i++ = r * sectors + (s+1);
    *i++ = (r+1) * sectors + (s+1);
    *i++ = (r+1) * sectors + s;
  }

	glGenBuffers(1, &vbo[INDEX_BUFFER]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * faces * sizeof(GLuint), i, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray (3);
	delete[] i;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	*/
}

void SolidSphere::draw (GLfloat x, GLfloat y, GLfloat z, GLuint pid)
{
    glBindVertexArray(vao);
    GLuint loc = glGetUniformLocation(pid, "modelMatrix");
    glm::mat4 model_matrix(r, 0, 0, 0, 0, r, 0, 0, 0, 0, r, 0, x, y, z, 1);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

    glEnableVertexAttribArray (0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[VERTEX_BUFFER]);
	  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	  glBindBuffer(GL_ARRAY_BUFFER, vbo[NORMAL_BUFFER]);
	  glEnableVertexAttribArray (2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //glDrawArrays(GL_TRIANGLES, 0, 3 * SPHERE_NUM_INDICES);
    glDrawElements(GL_TRIANGLES, SPHERE_NUM_INDICES, GL_UNSIGNED_INT, this->Indices);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(2);

    /*
    glBindVertexArray(vao);
    GLuint loc = glGetUniformLocation(pid, "modelMatrix");
    glm::mat4 model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_matrix));

	  glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, NULL);
	  glBindVertexArray(0);
    **/
}
