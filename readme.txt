# HTTP Library - Communication Protocols - Project 3

Comenzile se citesc succesiv de la consola. Inputul aditional necesar de comenzi va fi cerut secvential.

Pentru afisarea raspunsurile se extrage din raspuns codul si statusul returnate de server.

Jsonul, session id-ul si JWT tokenul sunt extrase cautandu-se secventa de caractere aferenta. Caracterele suplimentare din erori se gasesc si sterg folosind regex.

Functiile de nivel transport au fost preluate din scheletul de laborator si modificate pentru c
++.

Pentru parsare json-urilor am folosit libraria nlohmann. Am ales sa o folosesc deoarece este cea recomandata in fisierul cu cerinta.

## Comenzi

**register**

+username+password

Se citesc de la tastatura usernameul si parola. Se compune un json cu aceste campuri care se trimite la server printr-o cerere POST. Se afiseaza eroarea in cazul in care exista si succes in cazul in care nu.

**login**

+username+password

Se citesc de la tastature usernameul si parola. Se verifica daca userul nu este cumva deja loggat. Se compune un json cu datele citite si se trimite la server o cerere POST. Se afiseaza eroarea in cazul in care exista si succes in cazul in care nu.

Se extrage cookieul ce contine session id.

**enter_library**

Se trimite un request de tip GET care are ca cookie session id-ul. Se afiseaza eroarea in cazul in care exista si succes in cazul in care nu.

Se extrage JWT tokenul care ne confirma accesul.

**get_books**

Se trimite la server un request de tip GET la care este adaugat tokenul de acces. In cazul unei erori se va afisa, altfel se afiseaza un json array cu cartile existente.

Este necesara conectarea la librarie.

**get_book**

+id

Se trimite la server un request de tip GET pentru cartea id la care este adaugat tokenul de acces. In cazul unei erori se va afisa, altfel se afiseaza cartea cu id-ul dat intr-un mod mai detaliat.

Este necesara conectarea la librarie. Se verifica daca id este integer inainte de a se executa requestul.

**add_book**

+title+authoir+genre+page_count

Se citesc de la tastatura datele necesare si se compune un json. Se trimite un request de tip POST la care se adauga tokenul de acces. Se afiseaza eroarea in cazul in care exista si succes in cazul in care nu.

Este necesara conectarea la librarie. Se verifica daca page_count este integer inainte de a se executa requestul.

**delete_book**

+id

Se trimite la server un request de tip DELETE pentru cartea id la care este adaugat tokenul de acces. In cazul unei erori se va afisa, altfel se afiseaza cartea cu id-ul dat intr-un mod mai detaliat.

Este necesara conectarea la librarie. Se verifica daca id este integer inainte de a se executa requestul.

**logout**

Se trimite la server un GET request cu cookie session id. Se reseteaza session id-ul si JWT tokenul. Se afiseaza eroarea in cazul in care exista si succes in cazul in care nu.