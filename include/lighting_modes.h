#ifndef __LIGHTING_MODES_H__
#define __LIGHTING_MODES_H__

/* TODO:
class Rain;
class FlyingCube;
*/

class Mode {
    bool start_reset_to {LOW};

  protected:
    uint8_t pins_counter {0};
    uint32_t timer {millis()};
    uint8_t phase = 0;
    Mode(bool start_all_pins_to = LOW)
        : start_reset_to(start_all_pins_to), pins_counter(0), timer(millis()), phase(0)
    {
    }

  public:
    void reset_pin_state() const;
    virtual void set_to_start();
    virtual void mode() = 0;

    virtual ~Mode() {}
};

class AllLeds : public Mode {
    bool switch_led_to {HIGH};

  public:
    AllLeds() : Mode(LOW), switch_led_to(HIGH) {}
    void mode() override;
    void set_to_start() override;
};

class HorizontalLayerByLayer : public Mode {
  public:
    HorizontalLayerByLayer() : Mode(LOW) {}
    void mode() override;
};

class VerticalLayerByLayer : public Mode {
  public:
    VerticalLayerByLayer() : Mode(HIGH) {}
    void mode() override;
};

class Heart : public Mode {
    uint32_t knock_timer {millis()};
    uint8_t knocks_counter {0};

  public:
    Heart() : Mode(LOW), knock_timer {millis()} {}
    void mode() override;
    void set_to_start() override;
};

class CubeSmallBig : public Mode {
  public:
    CubeSmallBig() : Mode(LOW) {}
    void mode() override;
    void set_to_start() override;
};

class Rotator : public Mode {
    enum directions { right, down, left, up };
    static constexpr uint8_t num_directions = 4;

    class ColumnLight {
        uint8_t column_pin_number;

      public:
        ColumnLight(uint8_t pin_number = 0) : column_pin_number(pin_number) {}

        void move(directions direction);
        uint8_t get_new_pin_num(directions direction) const;
        uint8_t get_pin() const { return columnPins[column_pin_number]; }
    };

    ColumnLight lights[DIMENSION_H];

  public:
    Rotator() : Mode(LOW) {}
    void mode() override;
    void set_to_start() override;
};

#endif