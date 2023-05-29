enum class StatusCodes {
    SUCCESS = 0,
    CLOSE_HANDLE_ERROR = -1,
    INVALID_HANDLE_ERROR = -2,
    READ_ERROR = -3,
    WRITE_ERROR = -4,
    GET_STATE_ERROR = -5,
    SET_STATE_ERROR = -6,
    SET_TIMEOUT_ERROR = -7,
    BUFFER_ERROR = -8,
    NOT_FOUND_ERROR = -9
};

#define status(status) static_cast<int>(status)
