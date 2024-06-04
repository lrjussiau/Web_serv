#include "../inc/Utils.hpp"


BlockParser::BlockParser(const std::string& inputData) : data(inputData), pos(0) {}

std::string BlockParser::getNextBlock() {
    std::string token;
    int braceDepth = 0;
    bool capture = false;

    for (; pos < data.length(); ++pos) {
        char ch = data[pos];
        if (ch == '{') {
            if (braceDepth == 0) {
                capture = true;
            }
            braceDepth++;
        }
        if (capture) {
            token += ch;
        }
        if (ch == '}') {
            braceDepth--;
            if (braceDepth == 0) {
                capture = false;
                pos++;
                break;
            }
        }
    }
	return token;
}

std::string getTime(){
	std::time_t currentTime = std::time(0);
    char buffer[80];
	std::tm* gmtTime = std::gmtime(&currentTime);

	std::strftime(buffer, 80, "%a, %d %B %Y %H:%M:%S GMT", gmtTime);
	return std::string (buffer);
}

PathType getPathType(const std::string& path) {
	struct stat pathStat;
	if (stat(path.c_str(), &pathStat) != 0) {
		return PATH_NOT_FOUND;
	}
	if (S_ISREG(pathStat.st_mode)) {
		return PATH_IS_FILE;
	}
	if (S_ISDIR(pathStat.st_mode)) {
		return PATH_IS_DIRECTORY;
	}
	return PATH_NOT_FOUND;
}

void    setNonBlocking(int fd, int server) {
    struct linger	sl;
	int 			opt = 1;

    sl.l_onoff = 1;
    sl.l_linger = 0;
    if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
    if (server){
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
	    if (setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
		    perror("setsockopt SO_REUSEPORT");
		    exit(EXIT_FAILURE);
	    }
    }
    else{
        if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        if (setsockopt(fd, SOL_SOCKET, SO_LINGER, &sl, sizeof(sl))){
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
    }
    return;
}

std::string generateSessionId(void){
    size_t length = 32;
    // Create a random device and a Mersenne Twister engine
    std::random_device rd;
    std::mt19937_64 eng(rd());

    // Create a distribution to generate random bytes
    std::uniform_int_distribution<unsigned long long> distr;

    // Create a string stream to hold the session ID
    std::ostringstream oss;
    for (size_t i = 0; i < length / 8; ++i) {
        oss << std::hex << std::setw(16) << std::setfill('0') << distr(eng);
    }

   return oss.str().substr(0, length); // Ensure the length matches exactly
}

void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";
    running = false;
    return;
}