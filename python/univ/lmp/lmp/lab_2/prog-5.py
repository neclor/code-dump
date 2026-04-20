# ------------------------------------------------------------------------
# Laboratoires de programmation mathématique et physique 1                
# ------------------------------------------------------------------------
# 
# Programme 5: Vecteurs vitesse et accélération, détection de gestes.
#
# *** CONSIGNES ***: Ne modifier que les fonctions
#                        deplacer_pol(),
#                        dessiner_vecteur(),
#                        initialiser_calculs(),
#                        calculer_vitesse_acceleration_2d() et
#                        detecter_geste()  !!!
#
# ------------------------------------------------------------------------

import math
import pygame
import sys

### Constante(s)

BLEU = (0, 0, 255)
JAUNEMIEL = (255, 192, 0)
NOIR = (0, 0, 0)
ROUGE = (255, 0, 0)
VERT = (0, 255, 0)

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

# *** A MODIFIER *********************************************************

def initialiser_calculs():
    global previous_position, previous_temps, previous_velocity
    previous_position, previous_temps, previous_velocity = ( (0, 0), 0, (0, 0) )

# *** A MODIFIER *********************************************************

def calculer_vitesse_acceleration_2d(position, temps_maintenant):
    x = 0
    y = 1

    global previous_position, previous_temps, previous_velocity

    delta_position = (position[x] - previous_position[x], position[y] - previous_position[y])
    delta_temps = temps_maintenant - previous_temps

    velocity = (delta_position[x] / delta_temps, delta_position[y] / delta_temps)
    delta_velocity = (velocity[x] - previous_velocity[x], velocity[y] - previous_velocity[y])

    acceleration = (delta_velocity[x] / delta_temps, delta_velocity[y] / delta_temps)

    previous_position = position
    previous_temps = temps_maintenant
    previous_velocity = velocity

    return velocity, acceleration

# *** A MODIFIER *********************************************************

def detecter_geste(vitesse, acceleration):
    x = 0
    y = 1

    dist_v = math.sqrt(vitesse[x] ** 2 + vitesse[y] ** 2)
    dist_a = math.sqrt(acceleration[x] ** 2 + acceleration[y] ** 2)
    a = math.atan2(acceleration[y], acceleration[x])

    if dist_v < 0.2 and dist_a > 0.002 and math.pi / 2 + math.radians(10) >= a and a >= math.pi / 2 - math.radians(10):

        return True


    return False

# ************************************************************************

def afficher_compteur():
    image = police.render(str(compteur), True, NOIR)
    fenetre.blit(image, (50, 50))
    return

def amortir(v, ancien_v, coefficient):
    return (ancien_v[0] * coefficient + v[0] * (1.0 - coefficient),
            ancien_v[1] * coefficient + v[1] * (1.0 - coefficient))

def traiter_mouvement(position):
    global premier_mouvement, ancienne_position, ancienne_acceleration
    global compteur, derniere_detection

    if premier_mouvement:
        premier_mouvement = False
    else:
        x, y = position
        
        # Amortissement pour lisser les mouvements.
        position = amortir(position, ancienne_position,
                           amortissement_position)

        t = pygame.time.get_ticks()
        v, a = calculer_vitesse_acceleration_2d(position, t)

        a = amortir(a, ancienne_acceleration, amortissement_acceleration)
        ancienne_acceleration = a

        if detecter_geste(v, a) and t > derniere_detection + 500:
            compteur += 1
            derniere_detection = t
            
        fenetre.fill(couleur_fond)

        afficher_compteur()
        
        pygame.draw.circle(fenetre, BLEU,
                           (int(position[0]), int(position[1])), 20)

        if doit_afficher_vitesse:
            dessiner_vecteur(fenetre, ROUGE, position,
                             (int(v[0] * facteur_vitesse),
                              int(v[1] * facteur_vitesse)))

        if doit_afficher_acceleration:
            dessiner_vecteur(fenetre, VERT, position,
                             (int(a[0] * facteur_acceleration),
                              int(a[1] * facteur_acceleration)))
            
        pygame.display.flip()

    ancienne_position = position        
    return

### Paramètre(s)

dimensions_fenetre = (800, 600)  # en pixels
images_par_seconde = 25

couleur_fond = JAUNEMIEL

amortissement_position = 0.7
amortissement_acceleration = 0.5
facteur_vitesse = 200
facteur_acceleration = 40000

### Programme

# Initialisation

pygame.init()

fenetre = pygame.display.set_mode(dimensions_fenetre)
pygame.display.set_caption("Programme 5");

horloge = pygame.time.Clock()
police  = pygame.font.SysFont("monospace", 36)

premier_mouvement = True

ancienne_acceleration = (0.0, 0.0)

doit_afficher_vitesse = True
doit_afficher_acceleration = True

compteur = 0

derniere_detection = -1000

fenetre.fill(couleur_fond)
pygame.display.flip()

# Boucle principale

initialiser_calculs()

while True:
    for evenement in pygame.event.get():
        if evenement.type == pygame.QUIT:
            pygame.quit()
            sys.exit();
        elif evenement.type == pygame.KEYDOWN:
            if evenement.key == pygame.K_a:
                doit_afficher_acceleration = not doit_afficher_acceleration
            elif evenement.key == pygame.K_v:
                doit_afficher_vitesse = not doit_afficher_vitesse

    traiter_mouvement(pygame.mouse.get_pos())        
    horloge.tick(images_par_seconde)
