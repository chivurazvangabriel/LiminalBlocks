# Liminal Blocks

Proiect realizat de Chivu Razvan Gabriel - grupa 152

# Ghid utilizare

- A se deschide in IDE si a se builda
- rulati .exe-ul generat

# Despre ce e vorba

Liminal Blocks este un joc simulat intr-un voxel engine, inspirat de Minecraft. Acesta pune personajul principal intr-o lume 3D liminala, familiara.
Scopul jocului este explorarea numeroaselor regiuni liminale si infricosatoare, intr-o liniste claustranta si deprimanta.

# Implementare

In main, se initializeaza mai multe Subsisteme care functioneaza ca niste clase manager, care gestioneaza diverse portiuni ale jocului.
Blocurile sunt stocate intr-o structura numita Chunk, iar Chunk-urile sunt stocate in clasa World.
Clasele au implementate functii intuitive care sunt folosite pentru a interactiona in mod intuitiv cu lumea din joc.
Partea vizuala este implementata folosind OpenGL, avand doua stagii pentru randare: Scena preprocesata si post process. Au fost implementate prin a trimite cate un VAO si un VBO la GPU pentru fiecare chunk, alaturi de date importante cum ar fi pozitii, normale, UV-uri etc. Dupa acest stagiu, rezultatul randat este trimis drept input ca texture stadiului de post process, randand un Quad cat tot ecranul si aplicand functii matematice in Fragment Shader.

Controale:
- WASD - miscare
- Space - saritura
- Q - sparge bloc
- E - pune bloc

Bibliografie: gemini (folosit doar in cateva locuri pur teretice sau de sintaxa cand vine vorba de OpenGL; logica este gandita tot de mine)
