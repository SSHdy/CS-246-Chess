#include "moves.hpp"
#include <string>
#include <sstream>
#include <optional>

#include "code_utils.inc"

E_Color MoveRecord::add_move(RawMove rm) {
	_moves.push_back(rm);

	return this->_curr_move = !this->_curr_move;
}

RawMove parse_move(const std::string& raw_command) {
	std::istringstream iss(raw_command);
	std::string cm;
	BoardCoor coor1 = BoardCoor(0, 0);
	BoardCoor coor2 = BoardCoor(0, 0);
	std::optional<E_PieceType> promo = nullopt;
	int count = 0;
	while (iss >> cm) {
		if (count == 0) {
			coor1.x = cm[0] - 'a' + 1;
			coor1.y = cm[1] - '0';
		}
		else if (count == 1){
			coor2.x = cm[0] - 'a' + 1;
			coor2.y = cm[1] - '0';
		}
		else
		{
			promo = StringToPieceType(cm);
		}
		count++;
	}
	return RawMove(coor1, coor2, promo);
}

