#include <Arduino.h>
#include "config.h"
#include "lighting_modes.h"

// Mode
void Mode::reset_pin_state() const
{
    for (int i = 0; i < DIMENSION_H; i++) {
        pinMode(layerPins[i], OUTPUT);
        digitalWrite(layerPins[i], start_reset_to);
    }
    for (int i = 0; i < DIMENSION_S; i++) {
        pinMode(columnPins[i], OUTPUT);
        digitalWrite(columnPins[i], start_reset_to);
    }
}

void Mode::set_to_start()
{
    pins_counter = 0;
    timer = millis();
    phase = 0;
}

// Режимы работы
void AllLeds::mode()
{
    if (pins_counter == DIMENSION_H) {
        if (millis() - timer >= 2 * TIME_STEP) {
            switch_led_to = !switch_led_to;
            pins_counter = 0;
        }
    } else {
        digitalWrite(layerPins[pins_counter++], switch_led_to);
        if (pins_counter == DIMENSION_H) {
            timer = millis();
        }
    }
}

void AllLeds::set_to_start()
{
    Mode::set_to_start();
    switch_led_to = !switch_led_to;
}

void HorizontalLayerByLayer::mode()
{
    if (millis() - timer < TIME_STEP)
        return;

    timer = millis();
    switch (phase) {
    case 0: // Включаем по порядку
        digitalWrite(layerPins[pins_counter], HIGH);
        pins_counter++;
        if (pins_counter == DIMENSION_H) {
            pins_counter = 0;
            phase = 1;
        }
        break;

    case 1: // Выключаем по порядку
        digitalWrite(layerPins[pins_counter], LOW);
        pins_counter++;
        if (pins_counter == DIMENSION_H - 1) {
            pins_counter = DIMENSION_H - 1;
            phase = 2;
        }
        break;

    case 2: // Включаем в обратном порядке
        if (pins_counter > 0) {
            digitalWrite(layerPins[pins_counter], HIGH);
            pins_counter--;
        } else if (pins_counter == 0) {
            digitalWrite(layerPins[pins_counter], HIGH);
            pins_counter = DIMENSION_H - 1;
            phase = 3;
        }
        break;

    case 3: // Выключаем в обратном порядке
        if (pins_counter > 0) {
            digitalWrite(layerPins[pins_counter], LOW);
            pins_counter--;
        } else if (pins_counter == 0) {
            phase = 0;
        }
        break;
    }
}

void VerticalLayerByLayer::mode()
{
    if (millis() - timer < TIME_STEP)
        return;

    timer = millis();

    switch (phase) {
    case 0: // Включаем вертикальные слои
        for (int i = pins_counter; i < DIMENSION_H + pins_counter; i++)
            digitalWrite(columnPins[i], LOW);
        pins_counter += DIMENSION_H;
        if (pins_counter == DIMENSION_S) {
            phase = 1;
            pins_counter = 0;
        }
        break;
    case 1: // Выключаем вертикальные слои
        for (int i = pins_counter; i < DIMENSION_H + pins_counter; i++)
            digitalWrite(columnPins[i], HIGH);
        pins_counter += DIMENSION_H;
        if (pins_counter == DIMENSION_S - DIMENSION_H) {
            phase = 2;
            pins_counter = DIMENSION_S - 1;
        }
        break;
    case 2: // Включаем в обратном порядке
        for (int i = pins_counter; i > pins_counter - DIMENSION_H; i--)
            digitalWrite(columnPins[i], LOW);
        if (pins_counter > DIMENSION_H)
            pins_counter -= DIMENSION_H;
        else {
            phase = 3;
            pins_counter = DIMENSION_S - 1;
        }
        break;
    case 3: // Выключаем в обратном порядке
        for (int i = pins_counter; i > pins_counter - DIMENSION_H; i--)
            digitalWrite(columnPins[i], HIGH);
        if (pins_counter > DIMENSION_H + DIMENSION_H)
            pins_counter -= DIMENSION_H;
        else {
            phase = 0;
            pins_counter = 0;
        }
        break;
    }
}

void Heart::mode()
{
    if (millis() - timer < TIME_HEART_PHASE && knocks_counter == MAX_KNOCKS)
        return;
    if (millis() - knock_timer < TIME_HEART_KNOCK && knocks_counter < MAX_KNOCKS)
        return;

    if (knocks_counter == MAX_KNOCKS) {
        timer = millis();
        knocks_counter = 0;
    }
    if (knocks_counter < MAX_KNOCKS)
        knock_timer = millis();

    switch (phase) {
    case 0: // Включаем по порядку
        digitalWrite(layerPins[pins_counter++], HIGH);
        if (pins_counter == DIMENSION_H)
            phase = 1;
        break;
    case 1: // Выключаем по порядку
        digitalWrite(layerPins[--pins_counter], LOW);
        if (pins_counter == 1) {
            phase = 0;
            knocks_counter++;
        }
        break;
    }
}

void Heart::set_to_start()
{
    Mode::set_to_start();
    knock_timer = millis();
    knocks_counter = 0;
    for (int i = DIMENSION_H / 2; i < DIMENSION_H; i++) {
        for (int j = DIMENSION_H / 2; j < DIMENSION_H; j++) {
            pinMode(columnPins[i * DIMENSION_H + j], INPUT);
        }
    }
}

void CubeSmallBig::mode()
{
    if (millis() - timer < TIME_STEP)
        return;

    timer = millis();

    if (phase == 0) {
        for (int i = 0; i < DIMENSION_S_BORDER; i++)
            pinMode(borderColumnPins[i], INPUT);

        digitalWrite(layerPins[0], LOW);
        digitalWrite(layerPins[DIMENSION_H - 1], LOW);

        phase = 1;
    } else {
        for (int i = 0; i < DIMENSION_S_BORDER; i++) {
            pinMode(borderColumnPins[i], OUTPUT);
            digitalWrite(borderColumnPins[i], LOW);
        }
        digitalWrite(layerPins[0], HIGH);
        digitalWrite(layerPins[DIMENSION_H - 1], HIGH);

        phase = 0;
    }
}

void CubeSmallBig::set_to_start()
{
    Mode::set_to_start();
    for (int i = 1; i < DIMENSION_H - 1; i++)
        digitalWrite(layerPins[i], HIGH);
}

void Rotator::mode()
{
    if (millis() - timer < TIME_STEP)
        return;
    timer = millis();

    for (int i = 0; i < DIMENSION_H / 2; i++) {
        if (pins_counter > i && pins_counter < DIMENSION_H - i) {

            /* Направление выведено так:
            phase -- [right, down, left, up]
            ((phase + 2) % num_directions) -- [left, up, right, down]
            */
            lights[i].move((directions) phase);
            lights[DIMENSION_H - 1 - i].move((directions) ((phase + 2) % num_directions));
        }
    }

    /*
    (phase + 1) % 2) - для чередования HIGH, LOW
    Для чередования pins_counter, DIMENSION_H - pins_counter:
    (phase % 2) * DIMENSION_H + (1 - 2 * (phase % 2)) * pins_counter
    */
    digitalWrite(layerPins[(phase % 2) * DIMENSION_H + (1 - 2 * (phase % 2)) * pins_counter],
                 (phase + 1) % 2);

    pins_counter++;
    if (pins_counter == DIMENSION_H) {
        phase++;
        pins_counter = 1;
    }
    if (phase == num_directions)
        phase = 0;
}

void Rotator::set_to_start()
{
    Mode::set_to_start();
    for (int i = 0; i < DIMENSION_S; i++)
        pinMode(columnPins[i], INPUT);

    for (int i = 0; i < DIMENSION_H; i++) {
        lights[i] = ColumnLight(i + DIMENSION_H * i);
        digitalWrite(lights[i].get_pin(), LOW);
        pinMode(lights[i].get_pin(), OUTPUT);
        digitalWrite(layerPins[0], HIGH);
    }
    pins_counter++;
}

void Rotator::ColumnLight::move(directions direction)
{
    pinMode(get_pin(), INPUT);
    column_pin_number = get_new_pin_num(direction);
    digitalWrite(get_pin(), LOW);
    pinMode(get_pin(), OUTPUT);
}

uint8_t Rotator::ColumnLight::get_new_pin_num(directions direction) const
{
    switch (direction) {
    case right:
        return column_pin_number + 1;
    case down:
        return column_pin_number + DIMENSION_H;
    case left:
        return column_pin_number - 1;
    case up:
        return column_pin_number - DIMENSION_H;
    default:
        return column_pin_number;
    }
}

void Rain::mode()
{
    if (millis() - timer < RAIN_STEP)
        return;

    timer = millis();
    if (phase == 0) {
        digitalWrite(layerPins[0], LOW);
        pinMode(columnPins[pin_number], INPUT);
        pin_number = random(DIMENSION_S);

        digitalWrite(columnPins[pin_number], LOW);
        pinMode(columnPins[pin_number], OUTPUT);
    } else {
        digitalWrite(layerPins[DIMENSION_H - phase], LOW);
    }

    digitalWrite(layerPins[DIMENSION_H - 1 - phase], HIGH);
    phase++;
    if (phase == DIMENSION_H)
        phase = 0;
}

void Rain::set_to_start()
{
    Mode::set_to_start();
    for (int i = 0; i < DIMENSION_S; i++)
        pinMode(columnPins[i], INPUT);
}

void FlyingCube::mode()
{
    if (millis() - timer < TIME_STEP)
        return;
    timer = millis();

    move(available_directions.get_random());
}

void FlyingCube::set_to_start()
{
    Mode::set_to_start();
    cube_vertex.x = 1;
    cube_vertex.y = 1;
    cube_vertex.z = 1;

    directions available_dir[6] = {west, east, north, south, down, up};
    for (int i = 0; i < 6; i++)
        available_directions.add(available_dir[i]);

    for (int i = 0; i < DIMENSION_S_BORDER; i++)
        pinMode(borderColumnPins[i], INPUT);

    digitalWrite(layerPins[cube_vertex.z], HIGH);
    digitalWrite(layerPins[cube_vertex.z + 1], HIGH);
}

void FlyingCube::move(directions direction)
{
    switch (direction) {
    case west:
        if (cube_vertex.x > 0 && cube_vertex.x < DIMENSION_H - 1) {
            // гасим два восточных столбца
            pinMode(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x + 1], INPUT);
            pinMode(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x + 1], INPUT);

            if (cube_vertex.x == DIMENSION_H - 2) // отошли от восточной границы
                available_directions.add(east);
            cube_vertex.x--;
            if (cube_vertex.x == 0) // упёрлись в западную границу
                available_directions.erase(west);
            // зажигаем два западных столбца
            digitalWrite(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x], LOW);
            pinMode(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x], OUTPUT);
            digitalWrite(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x], LOW);
            pinMode(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x], OUTPUT);
        }
        break;
    case east:
        if (cube_vertex.x >= 0 && cube_vertex.x < DIMENSION_H - 2) {
            // гасим два западных столбца
            pinMode(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x], INPUT);
            pinMode(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x], INPUT);

            if (cube_vertex.x == 0) // отошли от западной границы
                available_directions.add(west);
            cube_vertex.x++;
            if (cube_vertex.x == DIMENSION_H - 2) // упёрлись в восточную границу
                available_directions.erase(east);
            // зажигаем два восточных столбца
            digitalWrite(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x + 1], LOW);
            pinMode(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x + 1], OUTPUT);
            digitalWrite(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x + 1], LOW);
            pinMode(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x + 1], OUTPUT);
        }
        break;
    case north:
        if (cube_vertex.y > 0 && cube_vertex.y < DIMENSION_H - 1) {
            // гасим два южных столбца
            pinMode(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x], INPUT);
            pinMode(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x + 1], INPUT);

            if (cube_vertex.y == DIMENSION_H - 2) // отошли от южной границы
                available_directions.add(south);
            cube_vertex.y--;
            if (cube_vertex.y == 0) // упёрлись в северную границу
                available_directions.erase(north);
            // зажигаем два северных столбца
            digitalWrite(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x], LOW);
            pinMode(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x], OUTPUT);
            digitalWrite(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x + 1], LOW);
            pinMode(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x + 1], OUTPUT);
        }
        break;
    case south:
        if (cube_vertex.y >= 0 && cube_vertex.y < DIMENSION_H - 2) {
            // гасим два северных столбца
            pinMode(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x], INPUT);
            pinMode(columnPins[cube_vertex.y * DIMENSION_H + cube_vertex.x + 1], INPUT);

            if (cube_vertex.y == 0) // отошли от северной границы
                available_directions.add(north);
            cube_vertex.y++;
            if (cube_vertex.y == DIMENSION_H - 2) // упёрлись в южную границу
                available_directions.erase(south);
            // зажигаем два южных столбца
            digitalWrite(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x], LOW);
            pinMode(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x], OUTPUT);
            digitalWrite(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x + 1], LOW);
            pinMode(columnPins[(cube_vertex.y + 1) * DIMENSION_H + cube_vertex.x + 1], OUTPUT);
        }
        break;
    case down:
        if (cube_vertex.z > 0 && cube_vertex.z < DIMENSION_H - 1) {
            // гасим верхний слой
            digitalWrite(layerPins[cube_vertex.z + 1], LOW);

            if (cube_vertex.z == DIMENSION_H - 2) // отошли от верхней границы
                available_directions.add(up);
            cube_vertex.z--;
            if (cube_vertex.z == 0) // упёрлись в нижнюю границу
                available_directions.erase(down);
            // зажигаем нижний слой
            digitalWrite(layerPins[cube_vertex.z], HIGH);
        }
        break;
    case up:
        if (cube_vertex.z >= 0 && cube_vertex.z < DIMENSION_H - 2) {
            // гасим нижний слой
            digitalWrite(layerPins[cube_vertex.z], LOW);

            if (cube_vertex.z == 0) // отошли от нижней границы
                available_directions.add(down);
            cube_vertex.z++;
            if (cube_vertex.z == DIMENSION_H - 2) // упёрлись в верхнюю границу
                available_directions.erase(up);
            // зажигаем верхний слой
            digitalWrite(layerPins[cube_vertex.z + 1], HIGH);
        }
        break;

    default:
        break;
    }
}
