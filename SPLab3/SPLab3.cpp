#include <iostream>
#include <string>
#include <vector>
#include <regex>

enum class TokenType {
    Number,
    StringLiteral,
    Identifier,
    Operator,
    ReservedWord,
    Delimiter,
    Comment,
    Type,
    Function,
    Macro,
    Unknown
};

struct Token {
    std::string value;
    TokenType type;
};


std::string getColor(TokenType type) {
    switch (type) {
    case TokenType::Number: return "\033[0;32m";            // Green for numbers
    case TokenType::StringLiteral: return "\033[1;33m";     // Yellow for string literals
    case TokenType::Identifier: return "\033[1;36m";        // Cyan for identifiers
    case TokenType::Function: return "\033[1;38;5;208m";    // Orange for functions
    case TokenType::Operator: return "\033[1;38;5;206m";    // Light pink for operators
    case TokenType::ReservedWord: return "\033[1;34m";      // Light blue for reserved words
    case TokenType::Delimiter: return "\033[0;37m";         // White for delimiters
    case TokenType::Comment: return "\033[1;90m";           // Gray for comments
    case TokenType::Type: return "\033[0;33m";              // Brown for types
    case TokenType::Macro: return "\033[1;35m";             // Purple for macros
    default: return "\033[1;31m";                           // Default color for unknown types
    }
}

const std::string RESET_COLOR = "\033[0m";



std::vector<Token> tokenize(const std::string& code) {
    std::vector<Token> tokens;

    
    std::regex number_regex(R"(\b0x[0-9a-fA-F]+|\b\d+(\.\d+)?\b)");
    std::regex string_literal_regex(R"("([^"\\]|\\.)*")");
    std::regex comment_regex(R"(//.*|/\*[\s\S]*?\*/)");
    std::regex operator_regex(R"([+\-*\/=<>!&|]{1,2}|[+=\-\/*%]|\.\.|::|\.\.=|&)");
    std::regex delimiter_regex(R"([{}();,.:])");
    std::regex identifier_regex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)");
    std::regex reserved_word_regex(R"(\b(fn|let|if|else|while|for|return|match|impl|trait|as|in|async|await|dyn|struct|enum|const|static|type|unsafe|mod|pub|self|crate|super|mut|continue|break|loop)\b)");
    std::regex library_regex(R"(use\s+([a-zA-Z_][a-zA-Z0-9_]*(::[a-zA-Z_][a-zA-Z0-9_]*)*)\s*;)");
    std::regex type_regex(R"(\b(i32|u32|i64|u64|f32|f64|String|&str|bool|char)\b)");
    std::regex function_regex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\s*\()");
    std::regex macro_regex(R"([a-zA-Z_][a-zA-Z0-9_]*!)");

    std::string::const_iterator search_start = code.cbegin();

    while (search_start != code.cend()) {
        std::smatch match;

        
        if (std::regex_search(search_start, code.cend(), match, comment_regex) && match.position() == 0) {
            tokens.push_back({ match.str(), TokenType::Comment });
        }
       
        else if (std::regex_search(search_start, code.cend(), match, string_literal_regex) && match.position() == 0) {
            tokens.push_back({ match.str(), TokenType::StringLiteral });
        }
        
        else if (std::regex_search(search_start, code.cend(), match, number_regex) && match.position() == 0) {
            tokens.push_back({ match.str(), TokenType::Number });
        }
        
        else if (std::regex_search(search_start, code.cend(), match, macro_regex) && match.position() == 0) {
            tokens.push_back({ match.str(), TokenType::Macro });
        }
       
        else if (std::regex_search(search_start, code.cend(), match, operator_regex) && match.position() == 0) {
            tokens.push_back({ match.str(), TokenType::Operator });
        }
        
        else if (std::regex_search(search_start, code.cend(), match, function_regex) && match.position() == 0) {
            std::string function_token = match.str();
            tokens.push_back({ function_token.substr(0, function_token.find('(')), TokenType::Function });
            tokens.push_back({ "(", TokenType::Delimiter });
        }
        
        else if (std::regex_search(search_start, code.cend(), match, reserved_word_regex) && match.position() == 0) {
            tokens.push_back({ match.str(), TokenType::ReservedWord });
        }
        
        else if (std::regex_search(search_start, code.cend(), match, library_regex) && match.position() == 0) {
            tokens.push_back({ "use", TokenType::ReservedWord });
            std::string library_name = match.str();
            size_t start = library_name.find(" ") + 1; 
            size_t end = library_name.find(";", start); 
            tokens.push_back({ library_name.substr(start, end - start), TokenType::Identifier });
        }
        
        else if (std::regex_search(search_start, code.cend(), match, type_regex) && match.position() == 0) {
            tokens.push_back({ match.str(), TokenType::Type });
        }
       
        else if (std::regex_search(search_start, code.cend(), match, delimiter_regex) && match.position() == 0) {
            tokens.push_back({ match.str(), TokenType::Delimiter });
        }
        
        else if (std::regex_search(search_start, code.cend(), match, identifier_regex) && match.position() == 0) {
            tokens.push_back({ match.str(), TokenType::Identifier });
        }
        else {
            tokens.push_back({ std::string(1, *search_start), TokenType::Unknown });
            ++search_start; 
            continue;
        }


        search_start += match.length();
    }

    return tokens;
}

void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << getColor(token.type) << token.value << RESET_COLOR << " ";
    }
    std::cout << std::endl;
}

void displayColorMeanings() {
    std::cout << "Token Color Meanings:\n";
    std::cout << "\033[1;32mGreen\033[0m: Numbers\n";
    std::cout << "\033[1;33mYellow\033[0m: String Literals\n";
    std::cout << "\033[1;36mCyan\033[0m: Identifiers\n";
    std::cout << "\033[1;38;5;208mOrange\033[0m: Functions\n";
    std::cout << "\033[1;38;5;206mLight Pink\033[0m: Operators\n";
    std::cout << "\033[1;34mLight Blue\033[0m: Reserved Words\n";
    std::cout << "\033[0;37mWhite\033[0m: Delimiters\n";
    std::cout << "\033[1;90mGray\033[0m: Comments\n";
    std::cout << "\033[0;33mBrown\033[0m: Types\n";
    std::cout << "\033[1;35mPurple\033[0m: Macros\n";
    std::cout << "\033[1;31mRed\033[0m: Unknown Tokens\n";
    std::cout << std::endl;
}

int main() {
    displayColorMeanings(); 

    std::string code = R"(
use std::io;

fn main() {
    let input = prompt_for_number("Enter a number:");

    if input % 2 == 10 {
        println!("{} - even.", input);
    } else {
        println!("{} - odd.", input);
    }

    let factorial = (1..=input).product::<i32>();
    println!("Factorial of {} is {}.", input, factorial);
}
//kuku
fn prompt_for_number(message: &str) -> i32 {
    println!("{}", message);
    let mut input = String::new();
    std::io::stdin()
        .read_line(&mut input)
        .expect("Failed to read input");

    input.trim().parse().expect("Please enter a valid number")
} 

)";

    auto tokens = tokenize(code);
    printTokens(tokens);
}
