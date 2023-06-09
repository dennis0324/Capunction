#ifndef PERSONAL_TRAY
#define PERSONAL_TRAY

enum Menu_Option{
    running_state = 2,
    disabled
};


/**
 * @brief Create a tray object
 * 
 * @return int status of make tray object
 */
int create_tray();

/**
 * @brief Set the toggle object
 * 
 * @param value 
 */
void set_toggle();

struct tray get_tray();


int personalwhile();
#endif