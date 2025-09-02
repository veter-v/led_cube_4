#ifndef __LIGHTNING_MODES_H__
#define __LIGHTNING_MODES_H__

/* TODO:
class VerticalLayerByLayer;
class Heart;
class Cube;
class Rain;
*/

class Mode {
    bool start_reset_to {LOW};

  protected:
    uint8_t counter {0};
    uint32_t timer {millis()};
    uint8_t phase = 0;
    bool switch_led_to {HIGH};

  public:
    Mode(bool start_all_pins_to = LOW)
        : start_reset_to(start_all_pins_to), counter(0), timer(millis()), phase(0), switch_led_to(!start_all_pins_to)
    {
    }
    void reset_pin_state() const;
    void set_to_start();
    virtual void mode() {}
};

class AllLeds : public Mode {
  public:
    AllLeds() : Mode(LOW) {}
    void mode();
};

class HorizontalLayerByLayer : public Mode {
  public:
    HorizontalLayerByLayer() : Mode(LOW) {}
    void mode();
};

#endif