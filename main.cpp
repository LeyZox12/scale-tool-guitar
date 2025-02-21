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
vec2 mousepos;
vector<string> notes = {"A", "a", "B", "C", "c", "D", "d", "E", "F", "f", "G", "g"};
vector<vector<int>> notesInt;
vector<vector<string>> notesChar;
vector<int> offsets = {7, 0, 5, 10, 1, 7};
vector<int> minor = {2, 1, 2, 2, 1, 2, 2};
vector<int> major = {2, 2, 1, 2, 2, 2, 1};
vector<string> scale;
vector<string> wanted;
string getSelectedNote(int charSize, vec2 mousepos);
void updateNotes(RenderWindow& window);
vector<pair<string, string>> findAllScales();

void start()
{
    window.setFramerateLimit(10);
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
    ui.addButton(vec2(300, 280), vec2(100, 50), findAllScales, "search");
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
            mousepos = vec2(Mouse::getPosition(window).x, Mouse::getPosition(window).y);
            if(e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left)
            {
                string result = getSelectedNote(40, mousepos);
                if(result != "none" && count(wanted.begin(), wanted.end(), result) == 0)
                    wanted.push_back(result);
                else if(result != "none")
                    wanted.erase(find(wanted.begin(), wanted.end(), result));

            }
        }
        window.clear(Color::Black);
        updateNotes(window);
        ui.displayElements(window);
        window.display();
    }
    return 0;
}

void updateNotes(RenderWindow& window)
{
    int charSize = 40;
    Text t;
    t.setFont(font);
    t.setCharacterSize(charSize);
    CircleShape circle(charSize / 2);
    for(int i = 0; i < 6; i++)
    {
        line[0].position = vec2(0, 5 + i * charSize);
        line[1].position = vec2(22 * charSize, 5 + i * charSize);
        //window.draw(line);
        for(int j = 0; j < 22; j++)
        {
            circle.setFillColor(Color(50, 50, 50));
            if(count(wanted.begin(), wanted.end(), notesChar[i][j]) == 1)
                circle.setFillColor(Color(100, 50, 50));
            circle.setPosition(vec2(j * charSize, i * charSize));
            t.setFillColor(count(scale.begin(), scale.end(), notesChar[i][j]) == 1 ? Color::Cyan : Color::White);
            t.setPosition(vec2(j * charSize + charSize / 6, i * charSize - charSize / 6));
            t.setString(notesChar[i][j]);
            window.draw(circle);
            window.draw(t);
        }
    }
}

vector<pair<string, string>> findAllScales()
{
    vector<pair<string, string>> scales;
    vector<string> currentScale;
    for(int t = 0; t < 2; t++)
        for(int n = 0; n < notes.size(); n++)
        {
            currentScale.clear();
            int currentOffset = 0;
            for(int i = 0; i < 7; i++)
            {
                currentScale.push_back(notes[(n + currentOffset) % notes.size()]);
                currentOffset += t == 0 ? minor[i] : major[i];
            }
            sort(wanted.begin(), wanted.end());
            sort(currentScale.begin(), currentScale.end());
            for(auto & c : wanted)
                cout << c;
            cout << endl;
            for(auto& c : currentScale)
                cout << c;
            cout << endl;
            string type = t == 0 ? "minor" : "major";
            if(wanted == currentScale)
                cout << "found one at " << notes[n] << " " << type << endl;
        }
    return scales;
}

string getSelectedNote(int charSize, vec2 mousepos)
{
    vec2 pos = vec2(floor(mousepos.x / charSize), floor(mousepos.y / charSize));
    if(pos.x > 0 && pos.x < 22 && pos.y > 0 && pos.y < 6)
        return (notesChar[pos.y][pos.x]);
    return "none";
}
