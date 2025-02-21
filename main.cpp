#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../../class/UIutils.h"

using namespace std;
using namespace sf;

typedef Vector2f vec2;

RenderWindow window(VideoMode::getDesktopMode(), "Scale Generator");
Event e;
Font font;
VertexArray line(LineStrip, 2);
UIutils ui;
vector<string> notes = {"A", "a", "B", "C", "c", "D", "d", "E", "F", "f", "G", "g"};
vector<vector<int>> notesInt;
vector<vector<string>> notesChar;
vector<int> offsets = {7, 0, 5, 10, 1, 7};
vector<int> minor = {2, 1, 2, 2, 1, 2, 2};
vector<int> major = {2, 2, 1, 2, 2, 2, 1};
vector<string> scale;

void start()
{
    font.loadFromFile("res/font.otf");
    ui.font = font;
    ui.addDropDown(vec2(0, 280), vec2(100, 50), notes, 3, "key");
    ui.addDropDown(vec2(100, 280), vec2(100, 50), {"minor", "major"}, 2, "key");
    ui.addButton(vec2(200, 280), vec2(100, 50),
                 []
                 {
                    scale.clear();
                    int currentOffset = 0;
                    for(int i = 0; i < 7; i++)
                    {
                        scale.push_back(notes[(ui.dropDowns[0].valIndex + currentOffset) % notes.size()]);
                        currentOffset += ui.dropDowns[1].valIndex == 0 ? minor[i] : major[i];
                    }
                 }, "Confirm");
    vector<int> row;
    for(int j = 0; j < 22; j++)
            row.push_back(j);
    for(int i = 0; i < 6; i++)
    {
        vector<string> strRow;
        for(int j = 0; j < 22; j++)
        {
            strRow.push_back(notes[(j + offsets[i]) % notes.size()]);
        }
        notesChar.push_back(strRow);
    }
        notesInt.push_back(row);
}

int main()
{
    start();
    while(window.isOpen())
    {
        while(window.pollEvent(e))
        {
            if(e.type == Event::Closed)
                window.close();
            ui.updateElements(e, window);
        }
        window.clear(Color::Black);

        Text t;
        t.setFont(font);

        for(int i = 0; i < 6; i++)
        {
            line[0].position = vec2(0, 5 + i * 40);
            line[1].position = vec2(22 * 40, 5 + i * 40);
            //window.draw(line);
            for(int j = 0; j < 22; j++)
            {
                t.setFillColor(count(scale.begin(), scale.end(), notesChar[i][j]) == 1 ? Color::Cyan : Color::White);
                t.setPosition(vec2(j * 40, i * 40));
                t.setString(notesChar[i][j]);
                window.draw(t);
            }
        }
        ui.displayElements(window);
        window.display();
    }
    return 0;
}
