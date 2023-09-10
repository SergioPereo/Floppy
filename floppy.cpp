#include "common.h"

int full_pool[28];

enum turn_type{player_turn=0,flop_turn=1};
enum end_type{left_end=0,right_end=1};

int get_left_side_piece(int piece){
    return (piece&0xFFFF);
}
int get_right_side_piece(int piece){
    return ((piece>>16)&0xFFFF);
}

int get_stored_left(int stored){
    return (stored&0xFF);
}

int get_stored_right(int stored){
    return ((stored>>8)&0xFF);
}

int get_stored_turn(int stored){
    return ((stored>>16)&0xF);
}
/*
Codes:
1: Illegal move, player doesn't have the piece,
2: Illegal move, player cannot place the piece in that end
200: Ok, the piece has been played
*/

class Player {
    protected:
        vector<int> hand;
        vector<int> opponent_pool;
        int pieces_opponent=7;
        vector<int>::iterator it;
    public:
        Player(){
            this->create_pool();
        }
        void print_hand(){
            for(auto x:hand){
                cout << x << endl;
            }
        }
        void print_opponent_pool(){
            for(auto x:opponent_pool){
                cout << x << endl;
            }
        }
        void create_pool(){
            for(int i=0;i<28;i++){
                opponent_pool.push_back(full_pool[i]);
            }
        }
        void remove_from_pool(int piece){
            remove(opponent_pool.begin(), opponent_pool.end(), piece);
        }
        void remove_from_hand(int piece){
            remove(hand.begin(), hand.end(), piece);
        }
        void add_to_hand(int piece){
            hand.push_back(piece);
            remove_from_pool(piece);
        }
        void add_to_pool(int piece){
            opponent_pool.push_back(piece);
        }
        int get_opponent_pieces(){
            return pieces_opponent;
        }
        void clear_player(){
            hand.clear();
            opponent_pool.clear();
            create_pool();
        }
        void opponent_play(){
            pieces_opponent--;
        }
        void opponent_steal(){
            pieces_opponent++;
        }
        void own_steal(int piece){
            add_to_hand(piece);
        }

        void play(int piece){
            remove_from_hand(piece);
        }

        void unplay(int piece){
            hand.push_back(piece);
        }

        bool has_piece(int piece){
            it=std::find(hand.begin(),hand.end(),piece);
            return it != hand.end();
        }
};

class AlphaBetaResponse {
    public:
        float heuristic = -5;
        vector<int> play;
        AlphaBetaResponse(int heuristic, vector<int> play){
            this->heuristic=heuristic;
            this->play=play;
        }
};

class Floppy: public Player {
    protected:
        int sleft=-1, sright=-1;
        turn_type turn=turn_type::flop_turn;
    
    private:

        vector<vector<int>> possible_plays(){
            vector<vector<int>> possible_plays;
            for(int piece: hand){
                int l_temp = get_left_side_piece(piece);
                int r_temp = get_right_side_piece(piece);
                if(l_temp == sleft || r_temp == sleft){
                    vector<int> play;
                    play.push_back(piece);
                    play.push_back(end_type::left_end);
                    possible_plays.push_back(play);
                }
                if(l_temp == sright || r_temp == sright){
                    vector<int> play;
                    play.push_back(piece);
                    play.push_back(end_type::right_end);
                    possible_plays.push_back(play);
                }
            }
            return possible_plays;
        }
        vector<vector<int>> opponent_possible_plays(){
            vector<vector<int>> possible_plays;
            for(int piece: opponent_pool){
                int l_temp = get_left_side_piece(piece);
                int r_temp = get_right_side_piece(piece);
                if(l_temp == sleft || r_temp == sleft){
                    vector<int> play;
                    play.push_back(piece);
                    play.push_back(end_type::left_end);
                    possible_plays.push_back(play);
                }
                if(l_temp == sright || r_temp == sright){
                    vector<int> play;
                    play.push_back(piece);
                    play.push_back(end_type::right_end);
                    possible_plays.push_back(play);
                }
            }
            return possible_plays;
        }

        void play_simulate(int piece, end_type end, turn_type turn){
            int left_temp=get_left_side_piece(piece), right_temp=get_right_side_piece(piece);
            if(sleft==left_end){
                if(sleft==left_temp){
                    sleft = right_temp;
                } else {
                    sleft = left_temp;
                }
            }else{
                if(sright==left_temp){
                    sright = right_temp;
                } else {
                    sright = left_temp;
                }
            }
            if(turn==player_turn){
                remove_from_pool(piece);
                opponent_play();
            } else {
                play(piece);
            }
        }

        void unplay_simulate(int piece, end_type end, turn_type turn){
            int left_temp=get_left_side_piece(piece), right_temp=get_right_side_piece(piece);
            if(sleft==left_end){
                if(sleft==left_temp){
                    sleft = right_temp;
                    
                } else {
                    sleft = left_temp;
                }
            }else{
                if(sright==left_temp){
                    sright = right_temp;
                } else {
                    sright = left_temp;
                }
            }
            if(turn==player_turn){
                add_to_pool(piece);
                opponent_steal();
            } else {
                unplay(piece);
            } 
        }

        string get_state(vector<int> played){
            string state = "";
            for(int i = 0 ; i < hand.size() ; i++){
                if(i==hand.size()-1)
                    state+=to_string(hand[i]);
                else
                    state+=to_string(hand[i])+",";
            }
            state += "|";
            for(int i = 0 ; i < opponent_pool.size() ; i++){
                if(i==opponent_pool.size()-1)
                    state+=to_string(opponent_pool[i]);
                else
                    state+=to_string(opponent_pool[i])+",";
            }
            state += "|";
            for(int i = 0 ; i < played.size() ; i++){
                if(i==played.size()-1)
                    state+=to_string(played[i]);
                else
                    state+=to_string(played[i])+",";
            }
        }

        AlphaBetaResponse alphabeta(int depth, int max_depth,int alpha, int beta, turn_type turn){
            vector<vector<int>> plays;
            if(turn==turn_type::flop_turn){
                plays = possible_plays();
            } else {
                plays = opponent_possible_plays();
            }
            for(vector<int> play: plays){
                
            }

        }
    public:
        AlphaBetaResponse alphabeta(int depth, int max_depth,int alpha, int beta, int end_left, int end_right, turn_type turn){
            sleft=end_left;
            sright=end_right;
            return alphabeta(depth, max_depth, alpha, beta, turn);
        }
};


class Game {
    protected:
        Player player;
        Floppy floppy;
        vector<int> game_track;
        int end_left=-1, end_right=-1;
    public:
        Game(){
            this->start_game();
        }
        Floppy get_floppy(){
            return floppy;
        }
        vector<int> generate_pieces_hands(int quantity_pieces){
            vector<int> hand;
            while(hand.size()<7){
                int r = rand()%28;
                std::vector<int>::iterator it;
                it=std::find(hand.begin(),hand.end(),r);
                if(it==hand.end()){
                    hand.push_back(r);
                }
            }
            return hand;
        }
        void start_game(){
            vector<int> aux = generate_pieces_hands(14);
            int i = 0;
            for(int piece_id : aux){
                if(i<7){
                    player.add_to_hand(full_pool[piece_id]);
                } else {
                    floppy.add_to_hand(full_pool[piece_id]);
                }
                i++;
            }
        }
        void restart_game(){
            player.clear_player();
            floppy.clear_player();
            this->start_game();
        }
        int play(int piece, end_type end, turn_type turn){
            int response = 0;
            bool has_error = false;
            if(turn==player_turn){
                if(!player.has_piece(piece)){
                    response=1;
                    has_error=true;
                }
            }
            if(!has_error && end_left==-1){
                end_left=get_left_side_piece(piece);
                end_right=get_right_side_piece(piece);
                if(turn==player_turn){
                    player.play(piece);
                    floppy.opponent_play();
                } else {
                    floppy.play(piece);
                    player.opponent_play();
                }
                response = 200;
            } else if (!has_error){
                int left_temp=get_left_side_piece(piece), right_temp=get_right_side_piece(piece);
                if(end==left_end){
                    if(end_left==left_temp){
                        end_left = right_temp;
                        if(turn==player_turn){
                            player.play(piece);
                            floppy.opponent_play();
                        } else {
                            floppy.play(piece);
                            player.opponent_play();
                        }
                        response = 200;
                    } else if (end_left==right_temp) {
                        end_left = left_temp;
                        if(turn==player_turn){
                            player.play(piece);
                            floppy.opponent_play();
                        } else {
                            floppy.play(piece);
                            player.opponent_play();
                        }
                        response = 200;
                    } else {
                        response = 2;
                        has_error = true;
                    }
                }else{
                    if(end_right==left_temp){
                        end_right = right_temp;
                        if(turn==player_turn){
                            player.play(piece);
                            floppy.opponent_play();
                        } else {
                            floppy.play(piece);
                            player.opponent_play();
                        }
                        response = 200;
                    } else if (end_right==right_temp) {
                        end_right = left_temp;
                        if(turn==player_turn){
                            player.play(piece);
                            floppy.opponent_play();
                        } else {
                            floppy.play(piece);
                            player.opponent_play();
                        }
                        response = 200;
                    } else {
                        response = 2;
                        has_error = true;
                    }
                }
            }
            return response;
        }
        
        void store_play(int piece, turn_type turn){
            int l_temp = get_left_side_piece(piece), r_temp=get_right_side_piece(piece);
            int s_temp = (l_temp | (r_temp<<8) | (turn << 16));
            game_track.push_back(s_temp);
        }
};

void initialize_pool(){
    int count=0;
    for(int i=0;i<7;i++){
        for(int j=i;j<7;j++){
            full_pool[count++]=(i | (j<<16));
        }
    }
}

void print_vector(vector<int> vec){
    for(auto& x:vec){
        cout << x << '\n';
    }
}

int main(){
    initialize_pool();
    Game game;
    
}