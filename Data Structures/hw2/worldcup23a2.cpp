#include "worldcup23a2.h"

world_cup_t::world_cup_t() :
    teamsTreeId(), teamsRankTreeAbility(), playersArray()
{}

world_cup_t::~world_cup_t()
{
    for (int i = 0; i < playersArray.m_arraySize; i++){
        l_Node<Player>* iterator = playersArray.m_array[i].m_head;
        for (int j = 0; j<playersArray.m_array[i].m_size; j++) {
            l_Node<Player>* lToDelete = iterator;
            iterator = iterator->m_next;
            if (iterator != nullptr) {
                iterator->m_prev = nullptr;
                lToDelete->m_next = nullptr;
            }
            uf_Node* ufToDelete = lToDelete->m_data->m_placeInTree;
            ufToDelete->m_father = nullptr;
            ufToDelete->m_team = nullptr;
            ufToDelete->m_data = nullptr;

            delete lToDelete->m_data;
            lToDelete->m_data = nullptr;
            delete lToDelete;
            delete ufToDelete;
        }
    }

    delete[] playersArray.m_array;
    teamsTreeId.deletePostOrder(teamsTreeId.m_root, false);
    teamsRankTreeAbility.deletePostOrder(teamsRankTreeAbility.m_root, true);

}

StatusType world_cup_t::add_team(int teamId)
{
    if (teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    if (teamsTreeId.treeIdFind(teamId)!= nullptr){
        return StatusType::FAILURE;
    }

    Team *newTeam;
    t_Node<Team>* newTeamNodeId;
    try {
        newTeam = new Team(teamId);
        newTeamNodeId = teamsTreeId.treeInsertById(newTeam);
        teamsRankTreeAbility.treeInsert(newTeam);
        }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    if (newTeamNodeId == nullptr) {
        delete newTeam;
        delete newTeamNodeId;
        return StatusType::FAILURE;
    }
    newTeamNodeId->m_data->m_rootPlayersTree = nullptr;
    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId <= 0){
        return StatusType::INVALID_INPUT;
    }

    t_Node<Team>* nodeTeamId = teamsTreeId.treeIdFind(teamId);
    if (nodeTeamId == nullptr) {
        return StatusType::FAILURE;
    }
    t_Node<Team>* nodeTeamAbility = teamsRankTreeAbility.treeObjectFind(nodeTeamId->m_data);
    if (nodeTeamAbility == nullptr) {
        return StatusType::FAILURE;
    }

    uf_Node* UnionRoot = nodeTeamId->m_data->m_rootPlayersTree;
    nodeTeamId->m_data->m_rootPlayersTree = nullptr;
    nodeTeamAbility->m_data->m_rootPlayersTree = nullptr;
    if (UnionRoot != nullptr && nodeTeamId->m_data->m_size != 0){
        UnionRoot->m_data->m_gamesPlayed += nodeTeamId->m_data->m_gamesPlayed;
        UnionRoot->m_team = nullptr;
    }
   Team* teamToDelete = nodeTeamAbility->m_data;

    nodeTeamAbility->m_data = nullptr;
    nodeTeamId->m_data = nullptr;


    teamsRankTreeAbility.treeDelete(nodeTeamAbility);
    teamsTreeId.treeDelete(nodeTeamId);

    delete teamToDelete;

    return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    if ((playerId <= 0) || (teamId <= 0) || (gamesPlayed < 0) || (cards < 0) || (!spirit.isvalid())) {
        return StatusType::INVALID_INPUT;
    }

    t_Node<Team> *teamNodeId = teamsTreeId.treeIdFind(teamId);
    if ((teamNodeId == nullptr) || (playersArray.findInArray(playerId) != nullptr)) {
        return StatusType::FAILURE;
    }
    Player *newPlayer;
    uf_Node* newNodePlayer;
    try {
        newPlayer = new Player(playerId, spirit, gamesPlayed, cards, goalKeeper);
        newNodePlayer = new uf_Node(newPlayer);
    }
    catch(const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    newPlayer->m_placeInTree = newNodePlayer;
    t_Node<Team>* teamNodeRank = teamsRankTreeAbility.treeObjectFind(teamNodeId->m_data);
    teamNodeId->m_data->updateAddPlayer(spirit, ability, goalKeeper);

   try {
        teamNodeRank->m_data = nullptr;
        teamsRankTreeAbility.treeDelete(teamNodeRank);
        teamsRankTreeAbility.treeInsert(teamNodeId->m_data);
    }
   catch(const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
   }
    playersArray.insert(newPlayer);
    newNodePlayer->insert(teamNodeId->m_data);
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{
    if ((teamId1==teamId2)||(teamId1<=0)||(teamId2<=0)){
        return StatusType::INVALID_INPUT;
    }

    t_Node<Team> *team1Node = teamsTreeId.treeIdFind(teamId1);
    t_Node<Team> *team2Node = teamsTreeId.treeIdFind(teamId2);

    if ((team1Node==nullptr)||(team2Node==nullptr)){
        return StatusType::FAILURE;
    }

    if ((!team1Node->m_data->m_goalKeeper)||(!team2Node->m_data->m_goalKeeper)){
        return StatusType::FAILURE;
    }

    team1Node->m_data->m_gamesPlayed+=1;
    team2Node->m_data->m_gamesPlayed+=1;

    int pointsAbility1,pointsAbility2;
    pointsAbility1=team1Node->m_data->m_points+team1Node->m_data->m_ability;
    pointsAbility2=team2Node->m_data->m_points+team2Node->m_data->m_ability;

    if (pointsAbility1>pointsAbility2){
        team1Node->m_data->m_points+=3;
        output_t<int> output(1);
        return output;
    }
    if (pointsAbility1<pointsAbility2){
        team2Node->m_data->m_points+=3;
        output_t<int> output(3);
        return output;
    }

    int spiritStrength1,spiritStrength2;
    spiritStrength1=team1Node->m_data->m_totalSpirit.strength();
    spiritStrength2=team2Node->m_data->m_totalSpirit.strength();

    if (spiritStrength1>spiritStrength2){
        team1Node->m_data->m_points+=3;
        output_t<int> output(2);
        return output;
    }
    if (spiritStrength1<spiritStrength2){
        team2Node->m_data->m_points+=3;
        output_t<int> output(4);
        return output;
    }
    team1Node->m_data->m_points+=1;
    team2Node->m_data->m_points+=1;
    output_t<int> output(0);
    return output;
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
    if (playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    Player* player = playersArray.findInArray(playerId);
    if (player == nullptr) {
        return StatusType::FAILURE;
    }
    int games = 0;
    uf_Node* iterator = player->m_placeInTree;
    Team* team = iterator->find();
    if (team != nullptr) {
        games = team->m_gamesPlayed;
    }
    while (iterator->m_father != nullptr) {
        games += iterator->m_data->m_gamesPlayed;
        iterator = iterator->m_father;
    }
    games += iterator->m_data->m_gamesPlayed;
    output_t<int> output(games);
    return output;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if ((playerId <= 0) || (cards < 0)) {
        return StatusType::INVALID_INPUT;
    }

    Player* player = playersArray.findInArray(playerId);
    if (player == nullptr){
        return StatusType::FAILURE;
    }

    uf_Node* playerNode = player->m_placeInTree;
    Team* team = playerNode->find();

    if (team == nullptr){
        return StatusType::FAILURE;
    }

    player->m_cards += cards;
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
    if (playerId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    if (playersArray.findInArray(playerId) == nullptr) {
        return StatusType::FAILURE;
    }
	int numCards = playersArray.findInArray(playerId)->m_cards;
    output_t<int> output(numCards);
	return output;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if (teamId<=0){
        return StatusType::INVALID_INPUT;
    }
    t_Node<Team> *teamNode = teamsTreeId.treeIdFind(teamId);
    if (teamNode==nullptr){
        return StatusType::FAILURE;
    }

    int points = teamNode->m_data->m_points;
    output_t<int> output(points);
    return output;
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i) {
    t_Node<Team>* iterator = teamsRankTreeAbility.m_root;
    if ((iterator == nullptr) || (i < 0) || (i >= iterator->m_rank)) {
        return StatusType::FAILURE;
    }
    int k = i;

    while (iterator != nullptr) {
        if ((k == 0) && iterator->m_left_son == nullptr) {
            output_t<int> output(iterator->m_data->m_id);
            return output;
        }
        else if (iterator->m_left_son == nullptr) {
            iterator = iterator->m_right_son;
            k -= 1;
        }
        else if (iterator->m_left_son->m_rank == k) {
            output_t<int> output(iterator->m_data->m_id);
            return output;
        }
        else if (iterator->m_left_son->m_rank > k) {
            iterator = iterator->m_left_son;
        }
        else if (iterator->m_left_son->m_rank < k) {
            k -= (iterator->m_left_son->m_rank + 1);
            iterator = iterator->m_right_son;

        }
    }
    return StatusType::FAILURE;
}


output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId) {
    if (playerId <= 0){
        return StatusType::INVALID_INPUT;
    }

    Player* player = playersArray.findInArray(playerId);
    if (player == nullptr){
        return StatusType::FAILURE;
    }


    uf_Node* iterator = player->m_placeInTree;
    Team* team = iterator->find();
    if (team == nullptr){
        return StatusType::FAILURE;
    }

    permutation_t partial_spirit (player->m_spirit);
    while (iterator->m_father != nullptr) {
        iterator = iterator->m_father;
        partial_spirit = iterator->m_data->m_spirit * partial_spirit;
    }

    output_t<permutation_t> output(partial_spirit);
    return output;
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
	if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2) {
        return StatusType::INVALID_INPUT;
    }

    t_Node<Team>* buyerNode = teamsTreeId.treeIdFind(teamId1);
    t_Node<Team>* boughtNode = teamsTreeId.treeIdFind(teamId2);

    if (buyerNode == nullptr || boughtNode == nullptr) {
        return StatusType::FAILURE;
    }

    Team* buyer = buyerNode->m_data;
    Team* bought = boughtNode->m_data;

    if (bought->m_size == 0) {
        remove_team(teamId2);
        return StatusType::SUCCESS;
    }
    else if (buyer->m_size == 0) {
        remove_team(teamId1);

        t_Node<Team>* nodeToDeleteID = teamsTreeId.treeIdFind(teamId2);
        t_Node<Team>* nodeToDeleteRank = teamsRankTreeAbility.treeObjectFind(nodeToDeleteID->m_data);
        Team* newTeam = nodeToDeleteID->m_data;

        nodeToDeleteID->m_data = nullptr;
        nodeToDeleteRank->m_data = nullptr;
        newTeam->m_id = teamId1;

        teamsTreeId.treeDelete(nodeToDeleteID);
        teamsTreeId.treeInsertById(newTeam);

        teamsRankTreeAbility.treeDelete(nodeToDeleteRank);
        teamsRankTreeAbility.treeInsert(newTeam);

        return StatusType::SUCCESS;
    }

    Team* newTeam = new Team;
    newTeam->m_totalSpirit = buyer->m_totalSpirit * bought->m_totalSpirit;

    newTeam->unite(buyer);
    newTeam->unite(bought);
    newTeam->m_id = buyer->m_id;

    uf_Node* ufPlaceNode = buyer->m_rootPlayersTree->union1(bought->m_rootPlayersTree);

    remove_team(teamId1);
    remove_team(teamId2);

    teamsTreeId.treeInsertById(newTeam);
    teamsRankTreeAbility.treeInsert(newTeam);

    ufPlaceNode->m_team = newTeam;
    newTeam->m_rootPlayersTree = ufPlaceNode;

    return StatusType::SUCCESS;
}