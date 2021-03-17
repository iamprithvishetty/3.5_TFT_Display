#include "utils.h"

class LCD_SHAKTI {
    public:
        LCD_SHAKTI(uint16_t LCD_ID, uint16_t wid, uint16_t heigh);
        // LCD_SHAKTI( uint16_t wid, uint16_t heigh);
        void Init_LCD(void);
        uint16_t Read_ID(void);
        void Set_Rotation(uint8_t r);
        void Set_Addr_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
        uint16_t Get_Width(void);
        uint16_t Get_Height(void);
        void Vert_Scroll(int16_t top, int16_t scrollines, int16_t offset);
        void Invert_Display(bool i);

    private:
        uint16_t width,height,lcd_model,HEIGHT,WIDTH;
        uint8_t rotation;
        void Start(uint16_t ID);
        void Reset(void);
        void Push_Command(uint16_t cmd, uint8_t *block, int8_t N);
};