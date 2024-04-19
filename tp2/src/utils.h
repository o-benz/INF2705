#ifndef UTILS_H
#define UTILS_H

double rand01();

void getGroupRandomPos(int groupId, const int N_ROWS, float& x, float& z);

// Chemin pour les textures de skybox
/*
const char* pathes[] = {
	"../textures/skybox/Daylight Box_Right.bmp",
	"../textures/skybox/Daylight Box_Left.bmp",
	"../textures/skybox/Daylight Box_Top.bmp",
	"../textures/skybox/Daylight Box_Bottom.bmp",
	"../textures/skybox/Daylight Box_Front.bmp",
	"../textures/skybox/Daylight Box_Back.bmp",
};
*/

// Dans l'initialisation dans le main:
/*
    const int N_ROWS = 7;
    const int N_GROUPS = N_ROWS * N_ROWS;

    glm::mat4 groupsTransform[N_GROUPS];
    
    glm::mat4 treeTransform[N_GROUPS];
    glm::mat4 rockTransform[N_GROUPS];
    glm::mat4 shroomTransform[N_GROUPS];

    // ...

    glm::vec3 playerPosition = glm::vec3(0);
    glm::vec2 playerOrientation = glm::vec2(0);

    // ...

    bool isFirstPersonCam = false;

*/

// Pour le positionnement/dessin des groupes, utilisez
// for (int i = 0; i < N_GROUPS; ++i)

// Pour les input de souris
// int x=0, y=0;
// w.getMouseMotion(x, y);

// Pour le clavier
// w.getKeyHold(Window::Key::W)


// Votre main loop devrait dessiner dans un ordre semblable
// afin de profiter de la réutilisation des shaders:

// Ruisseau   - partie 2
// Personnage (si besoin)
// Sol
// Modèles (autant que possible tous ensembles: tous les arbres, tous les rochés, tous les champignons)
// HUD (carré/coeur dans l'écran)
// Skybox - partie 2


#endif // UTILS_H
