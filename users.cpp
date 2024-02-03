// else {
//     close_conn = false;
//     while (true) {
//         data_bind = recv(fds[i].fd, buffer, sizeof(buffer), 0);
//         if (data_bind < 0) {
//             if (errno != EWOULDBLOCK) {
//                 perror("recv() failed");
//                 close_conn = true;
//             }
//             break;
//         }

//         if (data_bind == 0) {
//             // Connexion fermée
//             printf("Connexion fermée\n");
//             close_conn = true;
//             break;
//         }

//         int len = data_bind;
//         std::cout << len << " octets reçus\n";

//         // Vérifier si l'utilisateur est authentifié
//         bool authenticated = false;
//         // On suppose que les données d'authentification sont au format : nom_utilisateur\r\npseudo\r\nmot_de_passe\r\n
//         std::string authData(buffer, len);
//         std::size_t found = authData.find("\r\n");
//         if (found != std::string::npos) {
//             std::string username = authData.substr(0, found);
//             authData = authData.substr(found + 2);

//             found = authData.find("\r\n");
//             if (found != std::string::npos) {
//                 std::string nickname = authData.substr(0, found);
//                 authData = authData.substr(found + 2);

//                 found = authData.find("\r\n");
//                 if (found != std::string::npos) {
//                     std::string password = authData.substr(0, found);
//                     authData = authData.substr(found + 2);

//                     // Effectuer la logique d'authentification ici
//                     // Vérifier si le nom_utilisateur, le pseudo et le mot_de_passe sont valides
//                     if (/* condition d'authentification */) {
//                         authenticated = true;
//                         // Ajouter l'utilisateur authentifié à la liste des utilisateurs authentifiés
//                     } else {
//                         // Envoyer un message indiquant l'échec de l'authentification
//                         std::string authFailureMsg = "Échec de l'authentification !";
//                         send(fds[i].fd, authFailureMsg.c_str(), authFailureMsg.length(), 0);
//                     }
//                 }
//             }
//         }

//         if (authenticated) {
//             // L'utilisateur est authentifié, traiter la commande
//             // Analyser la commande à partir du reste de authData et effectuer les actions nécessaires
//             // ...

//             // Envoyer une réponse à l'utilisateur
//             std::string response = "Commande traitée avec succès";
//             send(fds[i].fd, response.c_str(), response.length(), 0);
//         }

//         if (close_conn) {
//             close(fds[i].fd);
//             fds[i].fd = -1;
//             compress_array = true;
//         }
//     }
// }