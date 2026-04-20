# ------------------------------------------------------------------------
# Laboratoires de programmation mathématique et physique 1                
# ------------------------------------------------------------------------
# 
# Programme 4: Affichage de vecteurs.
#
# *** CONSIGNES ***: Ne modifier que les fonctions
#                        deplacer_pol() et
#                        dessiner_vecteur()  !!!
#
# ------------------------------------------------------------------------

import math
import pygame
import sys

### Constante(s)

JAUNEMIEL = (255, 192, 0)
NOIR = (0, 0, 0)

A = 2
B = 5
C = 20

### Fonctions

# *** A MODIFIER *********************************************************

def deplacer_pol(point, distance, orientation):
    x, y = point
    x_2 = x + distance * math.cos(orientation)
    y_2 = y + distance * math.sin(orientation)

    return (x_2, y_2)

# *** A MODIFIER *********************************************************

def dessiner_vecteur(fenetre, couleur, p, v):
    x = 0
    y = 1

    dist_v = math.sqrt(v[x] ** 2 + v[y] ** 2)
    a = math.atan2(v[y], v[x])


    if dist_v >= C:
        p_4 = (p[x] + v[x], p[y] + v[y])

        p_1 = deplacer_pol(p, A, a - math.pi / 2)
        p_7 = deplacer_pol(p, A, a + math.pi / 2)

        p_c = deplacer_pol(p, dist_v - C, a)

        p_2 = deplacer_pol(p_c, A, a - math.pi / 2)
        p_6 = deplacer_pol(p_c, A, a + math.pi / 2)

        p_3 = deplacer_pol(p_2, B, a - math.pi / 2)
        p_5 = deplacer_pol(p_6, B, a + math.pi / 2)

        polygon = [p_1, p_2, p_3, p_4, p_5, p_6, p_7]
    
    else:
        p_3 = (p[x] + v[x], p[y] + v[y])
        p_1 = deplacer_pol(p_3, C, a + math.pi)
        p_2 = deplacer_pol(p_1, A + B, a - math.pi / 2)
        p_4 = deplacer_pol(p_1, A + B, a + math.pi / 2)
        polygon = [p_1, p_2, p_3, p_4]

    pygame.draw.polygon(fenetre, couleur, polygon)
    
# ************************************************************************

def traiter_clic(position, bouton):
    global premier_clic, ancienne_position

    if bouton == 3:
        premier_clic = True
        fenetre.fill(couleur_fond)
        return

    if bouton != 1:
        return
    
    if premier_clic:
        premier_clic = False
    else:
        dessiner_vecteur(fenetre, NOIR, ancienne_position,
                         (position[0] - ancienne_position[0],
                          position[1] - ancienne_position[1]))
                         
    ancienne_position = position
    return

### Paramètre(s)

dimensions_fenetre = (800, 600)  # en pixels
images_par_seconde = 25

### Programme

# Initialisation

pygame.init()

fenetre = pygame.display.set_mode(dimensions_fenetre)
pygame.display.set_caption("Programme 4");

horloge = pygame.time.Clock()
couleur_fond = JAUNEMIEL

premier_clic = True

fenetre.fill(couleur_fond)

# Boucle principale

while True:
    for evenement in pygame.event.get():
        if evenement.type == pygame.QUIT:
            pygame.quit()
            sys.exit();
        elif evenement.type == pygame.MOUSEBUTTONDOWN:
            traiter_clic(evenement.pos, evenement.button)

    pygame.display.flip()
    horloge.tick(images_par_seconde)
