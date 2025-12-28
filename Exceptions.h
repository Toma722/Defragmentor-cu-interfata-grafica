#ifndef OOP_EXCEPTIONS_H
#define OOP_EXCEPTIONS_H
#include <exception>
#include <string>


class Exceptions : public std::exception {
    protected:
        std::string message;
    public:
        explicit Exceptions(std::string message) : message(std::move(message)) {};

        [[nodiscard]] const char* what() const noexcept override { return message.c_str();}
};

class DiskFullException : public Exceptions {
    public:
        DiskFullException() : Exceptions("Discul este plin!"){}
};

class SecurityException : public Exceptions {
    public:
        SecurityException() : Exceptions("Fisierul este protejat!"){}
};

class CorruptedDataException : public Exceptions {
    public:
        CorruptedDataException() : Exceptions("Date corupte!"){}
};


#endif //OOP_EXCEPTIONS_H