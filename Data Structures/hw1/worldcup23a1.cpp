#include "worldcup23a1.h"

world_cup_t::world_cup_t() : playersTree(), playersTreebyId(), teamsTree(), validTeamsTree() {}


world_cup_t::~world_cup_t() {

    l_Node<Team>* teamListIterator=teamsTree.m_list.m_head;
    int size = teamsTree.m_list.m_size;
    for (int i=0;i<size;i++){
        t_Node<Player>* treeHeadNode = teamListIterator->m_place_in_tree->m_data->m_teamPlayersTree.m_root;
        teamListIterator->m_place_in_tree->m_data->m_teamPlayersTree.deletePostOrder(treeHeadNode,false);
        teamListIterator=teamListIterator->m_next;
    }
    l_Node<Team>* teamIdListIterator=teamsTree.m_list.m_head;
    for (int i=0;i<size;i++){
        t_Node<Player>* treeIdHeadNode = teamIdListIterator->m_place_in_tree->m_data->m_teamPlayersByIdTree.m_root;
        teamIdListIterator->m_place_in_tree->m_data->m_teamPlayersByIdTree.deletePostOrder(treeIdHeadNode,false);
        teamIdListIterator=teamIdListIterator->m_next;
    }

    validTeamsTree.deletePostOrder(validTeamsTree.m_root,false);
    playersTree.deletePostOrder(playersTree.m_root,false);
    teamsTree.deletePostOrder(teamsTree.m_root,true);
    playersTreebyId.deletePostOrder(playersTreebyId.m_root,true);

}


StatusType world_cup_t::add_team(int teamId, int points)
{
    if ((teamId <= 0)||(points < 0)){
        return StatusType::INVALID_INPUT;
    }
    if (teamsTree.treeIdFind(teamId)!=nullptr){
        return StatusType::FAILURE;
    }
    Team *newTeam;
    t_Node<Team>* newTeamNode;
    try {
        newTeam = new Team(teamId, points); // check allocation
        newTeamNode = teamsTree.treeInsertById(newTeam);
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    if (newTeamNode == nullptr){
        delete newTeam;
        delete newTeamNode;
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    t_Node<Team>* team = teamsTree.treeIdFind(teamId);
    if ((team == nullptr)||(team->m_data->m_size!=0)){
        return StatusType::FAILURE;
    }
    t_Node<Team>* validTeam = team->m_place_other_tree;
    if (validTeam!=nullptr){
        validTeam->m_place_other_tree=nullptr;
        validTeamsTree.treeDelete(validTeam);
    }
    delete (team->m_data);
    teamsTree.treeDelete(team);
    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper) {
    if ((playerId <= 0) || (teamId <= 0) || (gamesPlayed < 0) || (goals < 0) || (cards < 0)) {
        return StatusType::INVALID_INPUT;
    }
    if ((gamesPlayed == 0) && ((goals > 0) || (cards > 0))) {
        return StatusType::INVALID_INPUT;
    }

    t_Node<Team> *teamNode = teamsTree.treeIdFind(teamId);
    t_Node<Player> *newPlayerNode = playersTreebyId.treeIdFind(playerId);

    if ((teamNode == nullptr) || (newPlayerNode != nullptr)) {
        return StatusType::FAILURE;
    }

    Player *newPlayer;
    try {
        newPlayer = new Player(playerId, teamId, gamesPlayed, goals, cards, goalKeeper);

        newPlayer->m_teamNode = teamNode;
        newPlayer->m_gamesWithoutPlayer = teamNode->m_data->m_games;

        newPlayerNode = playersTreebyId.treeInsertById(newPlayer);
        t_Node<Player>* newPlayerNode1 = playersTree.treeInsert(newPlayer);

        newPlayerNode1->m_place_other_tree = newPlayerNode;
        newPlayerNode->m_place_other_tree = newPlayerNode1;


        t_Node<Player> *newPlayerNode2 = teamNode->m_data->m_teamPlayersTree.treeInsert(newPlayer);
        newPlayerNode2->m_place_other_tree = newPlayerNode;

        t_Node<Player> *newPlayerNode3 = teamNode->m_data->m_teamPlayersByIdTree.treeInsertById(newPlayer);
        newPlayerNode3->m_place_other_tree = newPlayerNode;

        teamNode->m_data->addPlayerStatsToTeam(newPlayer);

        if ((teamNode->m_place_other_tree == nullptr) && (teamNode->m_data->isValid())) {
            t_Node<Team>* teamNode1=validTeamsTree.treeInsert(teamNode->m_data);
            teamNode->m_place_other_tree = teamNode1;
            teamNode1->m_place_other_tree=teamNode;
        }
    }
    catch(const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId) {

    if (playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    t_Node<Player>* nodePlayer1 = playersTreebyId.treeIdFind(playerId);
    if (nodePlayer1 == nullptr){
        return StatusType::FAILURE;
    }

    t_Node<Player>* nodePlayer2 = nodePlayer1->m_place_other_tree;

    t_Node<Team>* nodeTeam = nodePlayer1->m_data->m_teamNode;

    t_Node<Player>* nodePlayer3 = nodeTeam->m_data->m_teamPlayersTree.treeObjectFind(nodePlayer1->m_data);

    t_Node<Player>* nodePlayer4 = nodeTeam->m_data->m_teamPlayersByIdTree.treeIdFind(playerId);

    int validBefore = nodeTeam->m_data->isValid();

    nodePlayer1->m_place_other_tree = nullptr;
    nodePlayer2->m_place_other_tree = nullptr;
    nodePlayer3->m_place_other_tree = nullptr;
    nodePlayer4->m_place_other_tree = nullptr;

    nodePlayer1->m_data->m_teamNode->m_data->removePlayerStatsFromTeam(nodePlayer1->m_data);
    try {
        delete (nodePlayer1->m_data);
        playersTree.treeDelete(nodePlayer2);
        playersTreebyId.treeDelete(nodePlayer1);
        nodeTeam->m_data->m_teamPlayersTree.treeDelete(nodePlayer3);
        nodeTeam->m_data->m_teamPlayersByIdTree.treeDelete(nodePlayer4);

        int validAfter = nodeTeam->m_data->isValid();

        if (validBefore != validAfter){
            t_Node<Team>* nodeTeam1=nodeTeam->m_place_other_tree;
            nodeTeam1->m_place_other_tree = nullptr;
            nodeTeam->m_place_other_tree = nullptr;
            nodeTeam1->m_data=nullptr;
            validTeamsTree.treeDelete(nodeTeam1);
        }
    }
    catch(const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,int scoredGoals, int cardsReceived){

    if ((playerId <= 0)||(gamesPlayed < 0)||(scoredGoals < 0)||(cardsReceived < 0)){
        return StatusType::INVALID_INPUT;
    }
    t_Node<Player>* playerNode = playersTreebyId.treeIdFind(playerId);
    if (playerNode == nullptr){
        return StatusType::FAILURE;
    }

    t_Node<Team>* teamNode = playerNode->m_data->m_teamNode;
    if (teamNode == nullptr){
        return StatusType::FAILURE;
    }
    try {
        Player *player = new Player
                (playerId, teamNode->m_data->m_id,
                 playerNode->m_data->m_gamesAlone,
                 playerNode->m_data->m_goals,
                 playerNode->m_data->m_cards,
                 playerNode->m_data->m_goalKeeper);
        player->m_gamesWithoutPlayer = playerNode->m_data->m_gamesWithoutPlayer;/////////////////////////////////////////////////////שינוי
        player->updateStats(gamesPlayed, scoredGoals, cardsReceived);

        player->m_teamNode = teamNode;
        bool validBefore = teamNode->m_data->isValid();
        remove_player(playerId);

        t_Node<Player> *player1Node = playersTree.treeInsert(player);
        t_Node<Player> *player2Node = playersTreebyId.treeInsertById(player);

        teamNode->m_data->addPlayerStatsToTeam(player);
        if (validBefore) {
            t_Node<Team> *teamNode1 = validTeamsTree.treeInsert(teamNode->m_data);
            teamNode->m_place_other_tree = teamNode1;
            teamNode1->m_place_other_tree = teamNode;
        }

        player1Node->m_place_other_tree = player2Node;
        player2Node->m_place_other_tree = player1Node;
        t_Node<Player> *player3Node = teamNode->m_data->m_teamPlayersTree.treeInsert(player);
        t_Node<Player> *player4Node = teamNode->m_data->m_teamPlayersByIdTree.treeInsertById(player);
        player3Node->m_place_other_tree = player2Node;
        player4Node->m_place_other_tree = player2Node;
    }
    catch(const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if ((teamId1 <= 0)||(teamId2 <= 0)||(teamId1 == teamId2)){
        return StatusType::INVALID_INPUT;
    }
    t_Node<Team>* team1Node=validTeamsTree.treeIdFind(teamId1);
    t_Node<Team>* team2Node=validTeamsTree.treeIdFind(teamId2);
    if ((team1Node == nullptr)||(team2Node == nullptr)){
        return StatusType::FAILURE;
    }
    team1Node->m_data->playMatch(team2Node->m_data);
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId) {
    if (playerId <= 0) {
        output_t<int> valueToReturn(StatusType::INVALID_INPUT);
        return valueToReturn;
    }
    t_Node<Player> *nodePlayer = playersTreebyId.treeIdFind(playerId);
    if (nodePlayer == nullptr) {
        output_t<int> valueToReturn(StatusType::FAILURE);
        return valueToReturn;
    }
    int numPlayedGames = (nodePlayer->m_data->m_gamesAlone) +
                         (nodePlayer->m_data->m_teamNode->m_data->m_games) -
                         (nodePlayer->m_data->m_gamesWithoutPlayer);
    output_t<int> valueToReturn(numPlayedGames);
    return valueToReturn;
}

output_t<int> world_cup_t::get_team_points(int teamId) {
    if (teamId <= 0) {
        output_t<int> valueToReturn(StatusType::INVALID_INPUT);
        return valueToReturn;
    }
    t_Node<Team> *nodeTeam = teamsTree.treeIdFind(teamId);
    if (nodeTeam == nullptr) {
        output_t<int> valueToReturn(StatusType::FAILURE);
        return valueToReturn;
    }
    int teamPoints = nodeTeam->m_data->m_points;
    output_t<int> valueToReturn(teamPoints);
    return valueToReturn;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
    if ((newTeamId<=0)||(teamId1<=0)||(teamId2<=0)||(teamId1==teamId2)){
        return StatusType::INVALID_INPUT;
    }

    t_Node<Team>* nodeTeam1 = teamsTree.treeIdFind(teamId1);
    t_Node<Team>* nodeTeam2 = teamsTree.treeIdFind(teamId2);
    t_Node<Team>* nodeNewTeam=teamsTree.treeIdFind(newTeamId);

    if ((nodeTeam1 == nullptr)||(nodeTeam2 == nullptr)){
        return StatusType::FAILURE;
    }
    if ((nodeNewTeam!=nullptr)&&(newTeamId!=teamId1)&&(newTeamId!=teamId2)){
        return StatusType::FAILURE;
    }
    try {
        int newTeamPoints = nodeTeam1->m_data->m_points + nodeTeam2->m_data->m_points;
        Team *newTeam = new Team(newTeamId, newTeamPoints); // check allocation
        nodeNewTeam = teamsTree.treeInsertById(newTeam);
        nodeNewTeam->m_data->mergeTeam(nodeTeam1->m_data, nodeTeam2->m_data);

        int length = nodeTeam1->m_data->m_teamPlayersTree.m_list.m_size +
                     nodeTeam2->m_data->m_teamPlayersTree.m_list.m_size;

        nodeNewTeam->m_data->m_teamPlayersTree.createFullTree(length);
        nodeNewTeam->m_data->m_teamPlayersTree.m_list.mergeListOther(&nodeTeam1->m_data->m_teamPlayersTree.m_list,
                                                                &nodeTeam2->m_data->m_teamPlayersTree.m_list);

        nodeNewTeam->m_data->m_teamPlayersByIdTree.createFullTree(length);
        nodeNewTeam->m_data->m_teamPlayersByIdTree.m_list.mergeList(&nodeTeam1->m_data->m_teamPlayersByIdTree.m_list,
                                                                    &nodeTeam2->m_data->m_teamPlayersByIdTree.m_list);

        l_Node<Player> *iterator = nodeNewTeam->m_data->m_teamPlayersTree.m_list.m_head;
        for (int i = 0; i < length; i++) {
            iterator->m_data->m_teamNode = nodeNewTeam;
            iterator = iterator->m_next;
        }

        nodeTeam1->m_data->m_size = 0;
        nodeTeam2->m_data->m_size = 0;


        nodeTeam1->m_data->m_teamPlayersTree.deletePostOrder(nodeTeam1->m_data->m_teamPlayersTree.m_root, false);
        nodeTeam1->m_data->m_teamPlayersByIdTree.deletePostOrder(nodeTeam1->m_data->m_teamPlayersByIdTree.m_root, false);
        delete nodeTeam1->m_data;

        nodeTeam2->m_data->m_teamPlayersTree.deletePostOrder(nodeTeam2->m_data->m_teamPlayersTree.m_root, false);
        nodeTeam2->m_data->m_teamPlayersByIdTree.deletePostOrder(nodeTeam2->m_data->m_teamPlayersByIdTree.m_root, false);
        delete nodeTeam2->m_data;

        this->validTeamsTree.treeDelete(nodeTeam1->m_place_other_tree);
        this->teamsTree.treeDelete(nodeTeam1);
        this->validTeamsTree.treeDelete(nodeTeam2->m_place_other_tree);
        this->teamsTree.treeDelete(nodeTeam2);



        int goodLeaves = nodeNewTeam->m_data->m_teamPlayersTree.numTreeLeaves(length);

        nodeNewTeam->m_data->m_teamPlayersTree.m_list.m_good_leaves = goodLeaves;
        nodeNewTeam->m_data->m_teamPlayersTree.m_list.m_iterator = nodeNewTeam->m_data->m_teamPlayersTree.m_list.m_head;
        nodeNewTeam->m_data->m_teamPlayersTree.addInorderList(nodeNewTeam->m_data->m_teamPlayersTree.m_root);
        nodeNewTeam->m_data->m_teamPlayersTree.m_list.m_iterator = nullptr;
        nodeNewTeam->m_data->m_teamPlayersTree.m_list.m_good_leaves = 0;

        nodeNewTeam->m_data->m_teamPlayersByIdTree.m_list.m_good_leaves = goodLeaves;
        nodeNewTeam->m_data->m_teamPlayersByIdTree.m_list.m_iterator = nodeNewTeam->m_data->m_teamPlayersByIdTree.m_list.m_head;
        nodeNewTeam->m_data->m_teamPlayersByIdTree.addInorderList(nodeNewTeam->m_data->m_teamPlayersByIdTree.m_root);
        nodeNewTeam->m_data->m_teamPlayersByIdTree.m_list.m_iterator = nullptr;
        nodeNewTeam->m_data->m_teamPlayersByIdTree.m_list.m_good_leaves = 0;

        if (nodeNewTeam->m_data->isValid()) {
            t_Node<Team> *nodeNewTeamValid = validTeamsTree.treeInsertById(nodeNewTeam->m_data);
            nodeNewTeamValid->m_place_other_tree = nodeNewTeam;
            nodeNewTeam->m_place_other_tree = nodeNewTeamValid;
        }
    }
    catch(const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }

    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if (teamId == 0) {
        output_t<int> valueToReturn(StatusType::INVALID_INPUT);
        return valueToReturn;
    }
    else if (teamId > 0) {
        t_Node<Team> *nodeTeam = teamsTree.treeIdFind(teamId);
        if (nodeTeam == nullptr) {
            output_t<int> valueToReturn(StatusType::FAILURE);
            return valueToReturn;
        }
        if (nodeTeam->m_data->m_size == 0) {
            output_t<int> valueToReturn(StatusType::FAILURE);
            return valueToReturn;
        }
        int playerId = nodeTeam->m_data->m_teamPlayersTree.m_list.m_rear->m_data->m_id;
        output_t<int> valueToReturn(playerId);
        return valueToReturn;
    }
    else {
        if (playersTree.m_root == nullptr) { //No players in game
            output_t<int> valueToReturn(StatusType::FAILURE);
            return valueToReturn;
        }
        int playerId = playersTree.m_list.m_rear->m_data->m_id;
        output_t<int> valueToReturn(playerId);
        return valueToReturn;
    }
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    if (teamId == 0) {
        output_t<int> valueToReturn(StatusType::INVALID_INPUT);
        return valueToReturn;
    }
    else if (teamId > 0) {
        t_Node<Team> *nodeTeam = teamsTree.treeIdFind(teamId);
        if (nodeTeam == nullptr) {
            output_t<int> valueToReturn(StatusType::FAILURE);
            return valueToReturn;
        }
        int numOfPlayers = nodeTeam->m_data->m_size;
        output_t<int> valueToReturn(numOfPlayers);
        return valueToReturn;
    }
    else {
        int numOfPlayers = playersTree.m_list.m_size;
        output_t<int> valueToReturn(numOfPlayers);
        return valueToReturn;
    }
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
    if (teamId == 0 || output == nullptr) {
        return StatusType::INVALID_INPUT;
    }
    else if (teamId > 0) {
        t_Node<Team> *nodeTeam = teamsTree.treeIdFind(teamId);
        if (nodeTeam == nullptr) {
            return StatusType::FAILURE;
        }
        if (nodeTeam->m_data->m_size == 0) {
            return StatusType::FAILURE;
        }
        l_Node<Player>* iterator = nodeTeam->m_data->m_teamPlayersTree.m_list.m_head;
        int  i = 0;
        while (iterator != nullptr) {
            output[i] = iterator->m_data->m_id;
            iterator = iterator->m_next;
            i++;
        }
        return StatusType::SUCCESS;
    }
    else {
        if (playersTree.m_root == nullptr) { //No players in game
            return StatusType::FAILURE;
        }
        l_Node<Player>* iterator = playersTree.m_list.m_head;
        int  i = 0;
        while (iterator != nullptr) {
            output[i] = iterator->m_data->m_id;
            iterator = iterator->m_next;
            i++;
        }
        return StatusType::SUCCESS;
    }
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    if ((playerId<=0)||(teamId<=0)){
        return StatusType::INVALID_INPUT;
    }
    if (playersTreebyId.m_list.m_size == 1){
        return StatusType::FAILURE;
    }
    t_Node<Team>* teamNode = teamsTree.treeIdFind(teamId);
    if (teamNode == nullptr){
        return StatusType::FAILURE;
    }

    t_Node<Player>* playerNode = teamNode->m_data->m_teamPlayersByIdTree.treeIdFind(playerId);
    if (playerNode == nullptr) {
        return StatusType::FAILURE;
    }
    playerNode = playerNode->m_place_other_tree;
    playerNode = playerNode->m_place_other_tree;

    if ((playerNode == nullptr)||(playerNode->m_data->m_teamNode != teamNode)){
        return StatusType::FAILURE;
    }

    l_Node<Player>* listPlace = playerNode->m_place_in_List;
    l_Node<Player>* beforePlace = listPlace->m_prev;
    l_Node<Player>* afterPlace = listPlace->m_next;
    if (beforePlace==nullptr){
        output_t<int> valueToReturn (afterPlace->m_data->m_id);
        return valueToReturn;
    }
    if (afterPlace==nullptr){
        output_t<int> valueToReturn (beforePlace->m_data->m_id);
        return valueToReturn;
    }
    output_t<int> valueToReturn (listPlace->m_data->closer(beforePlace->m_data,afterPlace->m_data));
    return valueToReturn;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId) {
    if (minTeamId < 0 || maxTeamId < 0 || maxTeamId < minTeamId) {
        output_t<int> valueToReturn(StatusType::INVALID_INPUT);
        return valueToReturn;
    }
    if (validTeamsTree.m_root == nullptr) {
        output_t<int> valueToReturn(StatusType::FAILURE);
        return valueToReturn;
    }
    if ((validTeamsTree.m_list.m_head->m_data->m_id > maxTeamId) || (validTeamsTree.m_list.m_rear->m_data->m_id < minTeamId)) {
        output_t<int> valueToReturn(StatusType::FAILURE);
        return valueToReturn;
    }

    l_Node<Team>* iterator;
    t_Node<Team>* newNodeTeam;

    if (validTeamsTree.treeIdFind(minTeamId) == nullptr) {
        Team minTeam(minTeamId, 0);
        newNodeTeam = validTeamsTree.treeInsert(&minTeam);
        iterator = newNodeTeam->m_place_in_List->m_next;
        validTeamsTree.treeDelete(newNodeTeam);
    }
    else {
        iterator = validTeamsTree.treeIdFind(minTeamId)->m_place_in_List;
    }

    l_Node<Team>* sizeCounterNode = iterator;
    int counterSize = 1;

    while ((sizeCounterNode->m_next != nullptr) && (minTeamId != maxTeamId)) {
        if (sizeCounterNode->m_next->m_data->m_id <= maxTeamId) {
            counterSize++;
            sizeCounterNode = sizeCounterNode->m_next;
        }
        else {
            break;
        }
    }

    int* array = new int[2*(counterSize + 1)];

    array[2*counterSize] = 0;
    array[2*counterSize + 1] = 0;

    for (int i = 0; i < 2*counterSize; i = i + 2) {
        array[i] = iterator->m_data->m_id;
        array[i + 1] = iterator->m_data->m_points + iterator->m_data->m_goals - iterator->m_data->m_cards;
        iterator = iterator->m_next;
    }

    int counter = counterSize;

    while (counter > 1) {
        int j = 0;
        int index = 0;
        while ((j < 2*counterSize) && (array[j] != 0)) {
            if (array[j + 2] == 0) {
                array[index] = array[j];
                array[index + 1] = array[j + 1];
            }
            else if (array[j + 3] == array[j + 1]) {
                if (array[j + 2] > array[j]) {
                    array[index] = array[j + 2];
                    array[index + 1] = array[j + 3] + array[j + 1] + 3;
                }
                else {
                    array[index] = array[j];
                    array[index + 1] = array[j + 1] + array[j + 3] + 3;
                }
            }
            else if (array[j + 3] > array[j + 1]) {
                array[index] = array[j + 2];
                array[index + 1] = array[j + 3] + array[j + 1] + 3;
            }
            else {
                array[index] = array[j];
                array[index + 1] = array[j + 1] + array[j + 3] + 3;
            }

            if (j == 0) {
                array[j + 2] = 0;
                array[j + 3] = 0;
            }
            else {
                array[j] = 0;
                array[j + 1] = 0;

                array[j + 2] = 0;
                array[j + 3] = 0;
            }
            j = j + 4;
            index = index + 2;
        }
        counter = (index/2);
    }

    output_t<int> valueToReturn(array[0]);
    delete[] array;
    return valueToReturn;
}