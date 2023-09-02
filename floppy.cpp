#include "common.h"

int full_pool[28];






class Player {
    protected:
        vector<int> hand;
        vector<int> opponent_pool;
        int pieces_opponent=7, left, right;
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
        void add_to_hand(int piece){
           
            hand.push_back(piece);
            remove(opponent_pool.begin(), opponent_pool.end(), piece);
           
        }
        int get_opponent_pieces(){
            return pieces_opponent;
        }
        void clear_player(){
            hand.clear();
            opponent_pool.clear();
            create_pool();
        }
};

class Floppy: public Player {

};


class Game {
    private:
        Floppy floppy;
    public:
        Game(){
            this->start_game();
        }
        Floppy get_floppy(){
            return floppy;
        }
        vector<int> generate_pieces_hands(){
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
            vector<int> aux = generate_pieces_hands();
            for(int i:aux){
                floppy.add_to_hand(full_pool[i]);
            }
        }
        void restart_game(){
            floppy.clear_player();
            this->start_game();
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

void check_pool(){
    cout << "Full: " << full_pool[4] << endl;
    cout << "First: " << (full_pool[4]&0xFFFF) << endl;
    cout << "Second: " << ((full_pool[4]>>16)&0xFFFF) << endl;
}

void print_vector(vector<int> vec){
    for(auto& x:vec){
        cout << x << '\n';
    }
}

int main(){
    initialize_pool();
    //check_pool();
    Game game;
    
}