class PietToken {

public:
	enum class Kind {
		Start, Value, Push, Pop, Add, Subtract, Multiply, Divide, Modulo, Not, Greater,
		Pointer, Switch, Duplicate, Roll, Input_Char, Input_Val, Output_Char, Output_Val, End, Unrecognised_Token
	};

	Kind kind = Kind::Start;
	int value;

	friend std::ostream& operator<<(std::ostream& os, const PietToken& tk) {
		switch (tk.kind)
		{
			case(Kind::Value):
				os << tk.value;
				break;
			case(Kind::Push):
				os << "Push";
				break;
			case(Kind::Pop):
				os << "Pop";
				break;
			case(Kind::Add):
				os << "Add";
				break;
			case(Kind::Subtract):
				os << "Subtract";
				break;
			case(Kind::Multiply):
				os << "Multiply";
				break;
			case(Kind::Divide):
				os << "Divide";
				break;
			case(Kind::Modulo):
				os << "Modulo";
				break;
			case(Kind::Not):
				os << "Not";
				break;
			case(Kind::Greater):
				os << "Greater";
				break;
			case(Kind::Pointer):
				os << "Pointer";
				break;
			case(Kind::Switch):
				os << "Switch";
				break;
			case(Kind::Duplicate):
				os << "Duplicate";
				break;
			case(Kind::Roll):
				os << "Roll";
				break;
			case(Kind::Input_Char):
				os << "Input_Char";
				break;
			case(Kind::Input_Val):
				os << "Input_Val";
				break;
			case(Kind::Output_Char):
				os << "Output_Char";
				break;
			case(Kind::Output_Val):
				os << "Output_Val";
				break;
			case(Kind::End):
				os << "End";
				break;
			case(Kind::Unrecognised_Token):
				os << "Unrecognised_Token";
				break;
			default:
				break;
		}
		return os;
	}
};