#  Système Embarqué d'Acquisition de Température (LM35)

> Un système embarqué temps réel développé en C pour l'acquisition, le traitement et l'affichage de données thermiques via le capteur LM35.


<img width="751" height="550" alt="Screenshot 2026-03-06 151657" src="https://github.com/user-attachments/assets/4d7f9c60-2572-4ecd-8a63-ef827e3b1b83" />

---

##  À propos du projet

Ce projet consiste en la conception et le développement d'un système embarqué complet capable de mesurer et d'afficher la température de manière continue. Il met en œuvre une configuration avancée et bas niveau des périphériques du microcontrôleur pour garantir un échantillonnage précis et une communication fluide, le tout sous de fortes contraintes de temps réel.

##  Fonctionnalités Principales

* **Acquisition de données :** Lecture analogique précise de la température via le capteur **LM35**.
* **Traitement Temps Réel :** Garantie d'un traitement ininterrompu des signaux sans perte d'information.
* **Communication Série :** Transmission stable et continue des données vers un moniteur ou un PC externe.
* **Affichage :** Restitution claire des mesures thermiques acquises.

---

##  Matériel et Technologies

* **Langage de programmation :** C
* **Microcontrôleur :** PIC18F5420 *(ou modèle équivalent)*
* **Composant externe :** Capteur de température LM35

** Périphériques configurés et optimisés :**
* **ADC (Convertisseur Analogique-Numérique) :** Utilisé pour convertir la tension analogique du capteur LM35 en valeur numérique exploitable.
* **Timers :** Paramétrés pour cadencer l'échantillonnage des signaux avec une précision temporelle stricte.
* **UART :** Configuré pour assurer une communication série stable (transmission des données de température vers un PC/Terminal).
* **SPI / I2C :** Prêts pour l'interfaçage avec d'autres modules externes ou écrans d'affichage.

---

##  Installation et Déploiement

Ce projet est conçu pour être compilé et flashé via les outils standards de Microchip.

1. **Cloner le dépôt :**
   git clone https://github.com/TrongNGUYEN1312/PIC18F4520.git
2. **Ouvrir le projet :**
   Importez le dossier du projet dans votre IDE (ex: MPLAB X IDE).
3. **Compilation :**
   Compilez le code source en utilisant le compilateur XC8.
4. **Flashage :**
   Téléversez le programme sur le microcontrôleur via un programmateur (ex: PICkit 3 / PICkit 4).
