
#include "CatchingLexerProxy.h"

std::unordered_set<std::string> map;

const std::string* lookupSymbol(const std::string &str){
    const auto &p = map.emplace(str);
    return &(*p.first);
}

bool CatchingLexerProxy::hasNextToken(Token &token) {
    int32_t startLine = line;
    int32_t startColumn = column;
    while (true) {
        if (current == 256) {
            if (finish) {
                return false;
            } else {
                accept = 0;
                finish = true;
                break;
            }
        }
        accept = -1;
        offset = 0;
        control.parse();
        if (accept == -1) {
            token.location.line = startLine;
            token.location.column = startColumn;
            offset = reader->getOffset();
            std::string msg = "_<-- char >";
            if (current == 256) {
                msg += "EOL";
            } else if (current == 0) {
                msg += "NULL";
            } else {
                msg += std::to_string((char) current);
            }
            msg += "< wrong!";
            errorObj = new Error(token.location, reader->readString(offset) + msg);
            error = true;
            return false;
        }

        for (uint32_t i = 0; i < offset; i++) {
            updatePosition(reader->get(i));
        }

        if (control.isHiding(accept)) {
            if (control.isCatching(accept)) {
                const auto &p = map.emplace(reader->readString(offset));
                token.value = &(*p.first);
            } else {
                token.value = nullptr;
            }
            reader->reset(offset);
            current = reader->peek();
            break;
        } else {
            reader->reset(offset);
            current = reader->peek();
            startLine = line;
            startColumn = column;
        }
    }
    token.location.line = startLine;
    token.location.column = startColumn;
    token.id = accept;
    return true;
}

void CatchingLexerProxy::updatePosition(char c) {
    if (c == '\r') {
        if (last == '\n') {
            last = 0;
        } else {
            column = 1;
            line++;
            last = c;
        }
    } else if (c == '\n') {
        if (last == '\r') {
            last = 0;
        } else {
            column = 1;
            line++;
            last = c;
        }
    } else {
        column++;
        last = c;
    }
}

void CatchingLexerProxy::reset(InputReader *reader) {
    Lexer::reset(reader);
    current = reader->peek();
    control.init(*this);
}

Error *CatchingLexerProxy::getError() {
    return errorObj;
}

bool CatchingLexerProxy::isError() {
    return error;
}