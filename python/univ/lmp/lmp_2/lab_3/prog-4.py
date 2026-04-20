# ------------------------------------------------------------------------
# Laboratoires de programmation mathématique et physique 2
# ------------------------------------------------------------------------
#
# Programme : 7 segments.
#
# ------------------------------------------------------------------------

import math
import pygame
import sys
import numpy as np

### Constante(s)

NOIR = (0, 0, 0)
GRIS = (200, 200, 200)
ROUGE = (255, 0, 0)


### Variables Globales
valeur_memorisee: int = 3


def dessiner_arduino(sortie_arduino, sortie_CD4511, sortie_bouton):
    fenetre.blit(image_arduino, pos_arduino)
    fenetre.blit(image_CD4511, pos_CD4511)
    fenetre.blit(image_bouton, pos_bouton)


    off_ard = 194
    off_cd = 15
    for i in range(0, 4):
        if sortie_arduino[i] == 0:
            couleur = NOIR
        else:
            couleur = ROUGE

        pygame.draw.line(fenetre, couleur, (pos_arduino[0] + 280, pos_arduino[1] + off_ard),
                        (pos_CD4511[0] + 7, pos_CD4511[1] + off_cd), 5)
        off_ard = off_ard + 14
        off_cd = off_cd + 19


    off_cd = 15
    off_aff = 27
    for i in range(0, 7):
        if sortie_CD4511[i] == 0:
            couleur = NOIR
        else:
            couleur = ROUGE
        pygame.draw.line(fenetre, couleur, (pos_afficheur[0], pos_afficheur[1] + off_aff),
                        (pos_CD4511[0] + 102, pos_CD4511[1] + off_cd), 5)
        off_aff = off_aff + 19
        off_cd = off_cd + 19

    connexion_bouton(sortie_bouton)



def dessiner_afficheur(sortie_CD4511):
    positions_barres = [[32, 14], [89, 20], [87, 88], [28, 150],
                        [17, 88], [19, 20], [30, 82]]
    fenetre.blit(image_afficheur, pos_afficheur)

    i = 0
    for barre in positions_barres:
        if sortie_CD4511[i] == 0:
            i = i + 1
            continue
        x_b = pos_afficheur[0] + int(round(barre[0]*(image_afficheur.get_width()/133)))
        y_b = pos_afficheur[1] + int(round(barre[1]*(image_afficheur.get_height()/192)))
        if i == 0 or i == 3 or i == 6:
            fenetre.blit(barre_horizontale, (x_b, y_b))
        else:
            fenetre.blit(barre_verticale, (x_b, y_b))
        i = i + 1
    return


def composant_CD4511(entree):
    tdv = np.array([
        [1, 1, 1, 1, 1, 1, 0],
        [0, 1, 1, 0, 0, 0, 0],
        [1, 1, 0, 1, 1, 0, 1],
        [1, 1, 1, 1, 0, 0, 1],
        [0, 1, 1, 0, 0, 1, 1],
        [1, 0, 1, 1, 0, 1, 1],
        [1, 0, 1, 1, 1, 1, 1],
        [1, 1, 1, 0, 0, 0, 0],
        [1, 1, 1, 1, 1, 1, 1],
        [1, 1, 1, 1, 0, 1, 1],
    ])
    index = entree[0] * 8 + entree[1] * 4 + entree[2] * 2 + entree[3]
    return tdv[index]


def sortie_memorisee():
    value = valeur_memorisee
    result = [0, 0, 0, 0]
    for i in range(4):
        result[3 - i] = value % 2
        value = value // 2
    return np.array(result)


def click():
    global valeur_memorisee
    valeur_memorisee += 1


def connexion_bouton(sortie_bouton):
    pygame.draw.line(fenetre, ROUGE if sortie_bouton == 1 else NOIR, pin_arduino, pin_bouton, 5)


### Paramètre(s)

dimensions_fenetre = (1100, 600)  # en pixels
images_par_seconde = 25

pos_arduino = (65, 84)
pos_CD4511 = (537, 263)
pos_afficheur = (818, 251)
pos_bouton = (537, 486)
pos_centre_bouton = (589, 521)
rayon_bouton = 18
pin_arduino = (pos_arduino[0] + 279, pos_arduino[1] + 353)
pin_bouton = (pos_bouton[0] + 13, pos_bouton[1] + 13)


### Programme

# Initialisation

pygame.init()

fenetre = pygame.display.set_mode(dimensions_fenetre)
pygame.display.set_caption("Programme 7 segments")

horloge = pygame.time.Clock()

image_afficheur_s = pygame.image.load('images/7_seg_s.png').convert_alpha(fenetre)
barre_verticale_s = pygame.image.load('images/vertical_s.png').convert_alpha(fenetre)
barre_horizontale_s = pygame.image.load('images/horizontal_s.png').convert_alpha(fenetre)
image_afficheur = pygame.image.load('images/7_seg.png').convert_alpha(fenetre)
barre_verticale = pygame.image.load('images/vertical.png').convert_alpha(fenetre)
barre_horizontale = pygame.image.load('images/horizontal.png').convert_alpha(fenetre)
image_arduino = pygame.image.load('images/arduino.png').convert_alpha(fenetre)
image_CD4511 = pygame.image.load('images/CD4511.png').convert_alpha(fenetre)
image_CD4028 = pygame.image.load('images/CD4028.png').convert_alpha(fenetre)
image_bouton = pygame.image.load('images/bouton.png').convert_alpha(fenetre)
couleur_fond = GRIS


# Boucle principale
sortie_bouton = 0

while True:
    temps_maintenant = pygame.time.get_ticks()
    for evenement in pygame.event.get():
        if evenement.type == pygame.USEREVENT:
            pass
        if evenement.type == pygame.QUIT:
            pygame.quit()
            sys.exit()

    button_pressed: bool = pygame.mouse.get_pressed()[0] == 1 and pygame.Vector2(pygame.mouse.get_pos()).distance_to(pos_centre_bouton) <= rayon_bouton

    if not button_pressed:
        sortie_bouton = 0

    elif sortie_bouton == 0:
        click()
        sortie_bouton = 1

    fenetre.fill(couleur_fond)

    sortie_CD4511 = composant_CD4511(sortie_memorisee())
    dessiner_afficheur(sortie_CD4511)
    dessiner_arduino(sortie_memorisee(), sortie_CD4511, sortie_bouton)

    pygame.display.flip()
    horloge.tick(images_par_seconde)
