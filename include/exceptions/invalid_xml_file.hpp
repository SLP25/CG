/**
 * @file invalid_xml_file.hpp
 * @brief Definition of InvalidXMLStructure exception class.
 */

#ifndef CG_INVALID_XML_FILE_HPP
#define CG_INVALID_XML_FILE_HPP

#include <exception>
#include <string>

/**
 * @class InvalidXMLStructure
 * @brief Exception class for invalid XML structure.
 */
class InvalidXMLStructure : public std::exception {

public:

    /**
     * @brief Constructor for InvalidXMLStructure exception class.
     * @param message A string message describing the exception.
     */
    InvalidXMLStructure(const std::string& message) : message_(message) {}

    /**
     * @brief Returns the message associated with the exception.
     * @return A pointer to a C-style string with the message.
     */
    virtual const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;

};

#endif //CG_INVALID_XML_FILE_HPP