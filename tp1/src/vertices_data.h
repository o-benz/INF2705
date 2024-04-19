#ifndef VERTICES_DATA_H
#define VERTICES_DATA_H

const GLfloat triVertices[] = {
    // Définir les coordonnées d'un triangle
    // en considérant le type de l'entrée du vertex shader.
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f,
};

const GLfloat squareVertices[] = {
    // Définir les coordonnées d'un carré
    // fait de deux triangles en considérant le type de
	// l'entrée du vertex shader.
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f,
    0.5f,  0.5f, 0.0f,
};

const GLfloat colorTriVertices[] = {
    // Définir les coordonnées d'un triangle
    // avec les attributs de position et couleur entrelacés
    // en considérant le type des entrées du vertex shader.
    -0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.2f,
    0.5f, -0.5f, 0.0f, 0.2f, 1.0f, 0.2f,
    0.0f, 0.5f, 0.0f, 0.2f, 0.2f, 1.0f,
};

const GLfloat colorSquareVertices[] = {
    // Définir les coordonnées d'un carré
    // fait de deux triangles avec les attributs de
    // position et couleur entrelacés en considérant le type
    // des entrées du vertex shader.
    -0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.2f,
    0.5f, -0.5f, 0.0f, 0.2f, 1.0f, 0.2f,
    -0.5f,  0.5f, 0.0f, 0.2f, 0.2f, 1.0f,
    0.5f, -0.5f, 0.0f, 0.2f, 1.0f, 0.2f,
    -0.5f,  0.5f, 0.0f, 0.2f, 0.2f, 1.0f,
    0.5f,  0.5f, 0.0f, 1.0f, 0.2f, 0.2f,
};


const GLfloat colorSquareVerticesReduced[] = {
    // Définir les coordonnées d'un carré
    // fait de deux triangles avec les attributs de
    // position et couleur entrelacés en considérant le type
    // des entrées du vertex shader. Il devrait y avoir
    // moins de données dans ce tableau.
    -0.5f, -0.5f, 0.0f, 1.0f, 0.2f, 0.2f,
    0.5f, -0.5f, 0.0f, 0.2f, 1.0f, 0.2f,
    -0.5f,  0.5f, 0.0f, 0.2f, 0.2f, 1.0f,
    0.5f,  0.5f, 0.0f, 1.0f, 0.2f, 0.2f,
};
const GLubyte indexes[] = {
    // Définir les indexes pour générer un
    // carré en réutilisant certaines vertices.
    0, 1, 2, // first triangle
    1, 2, 3  // second triangle
};


// Partie 2: On vous donne les vertices et indexes du cube.
// Inspirez-vous en pour les tableaux au-dessus au besoin.
const GLfloat cubeVertices[] = {
    // +z
    -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
     // -z
    -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
     // +x
     0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
     // -x
    -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
     // +y
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     // -y
    -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f,
};

const GLubyte cubeIndexes[] = {
     0,  1,  2,
     1,  3,  2,

     4,  5,  6,
     5,  7,  6,

     8,  9, 10,
     9, 11, 10,

    12, 13, 14,
    13, 15, 14,

    16, 17, 18,
    17, 19, 18,

    20, 21, 22,
    21, 23, 22,
};

#endif // VERTICES_DATA_H
