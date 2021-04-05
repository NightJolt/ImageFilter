#include "Input.h"
#include "Time.h"
#include "R.h"
#include "Node.h"
#include "Graph.h"

#include "globals.h"

int main() {
    std::ios::sync_with_stdio(false);

    fun::R::LoadResources();

    sf::Image image;
    sf::Image original_image;
    sf::Texture texture;
    sf::Sprite sprite;

    sf::RenderWindow window(sf::VideoMode(INIT_SCREEN_SIZE.x, INIT_SCREEN_SIZE.y), "ImageFilter");
    ImGui::SFML::Init(window);

    glob_window = &window;

    window.resetGLStates();
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(false);

    sf::View view(INIT_VIEW_ORIGIN, INIT_VIEW_SIZE);
    sf::Vector2f screen_resolution = sf::Vector2f(INIT_SCREEN_SIZE.x, INIT_SCREEN_SIZE.y);

    ImGuiIO& imgui_io = ImGui::GetIO();
    //imgui_io.FontGlobalScale = 2.f;
    imgui_io.IniFilename = nullptr;

    sf::Event event;

    char open_file_dir[64];
    std::fill(open_file_dir, open_file_dir + 64, 0);
    char out_file_dir[64];
    std::fill(out_file_dir, out_file_dir + 64, 0);
    bool is_file_loaded = false;

    float brightness_scale = 1.f;
    int sharpen_scale = 0;
    Col mul_col;
    bool greyscale = false;
    bool should_update = true;
    bool auto_update = true;

    Graph graph;
    graph.AddNode(Node::Add);
    graph.AddNode(Node::Add);
    graph.AddNode(Node::Add);
    graph.AddNode(Node::Add);
    graph.AddNode(Node::Add);
    graph.AddNode(Node::Add);
    graph.AddNode(Node::Join);
    graph.AddNode(Node::Split);
    graph.AddNode(Node::Split);
    graph.AddNode(Node::ImgPos);
    graph.AddNode(Node::ImgWriter);
    graph.AddNode(Node::ImgReader);
    graph.AddNode(Node::Grayscale);

    while (window.isOpen()) {
        fun::Input::Listen();
        fun::Time::Recalculate();

        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            switch (event.type) {
                case sf::Event::Closed:
                    window.close();

                    break;
                case sf::Event::MouseWheelMoved:
                    view.zoom(event.mouseWheel.delta > 0 ? 0.9f : 1.1f);

                    break;
                case sf::Event::Resized:
                    sf::Vector2f ratio = (sf::Vector2f)window.getSize() / screen_resolution;
                    screen_resolution = (sf::Vector2f)window.getSize();
                    view.setSize(view.getSize() * ratio);

                    break;
            }
        }

        ImGui::SFML::Update(window, sf::seconds(fun::Time::UnscaledDeltaTime()));

        if (fun::Input::Hold(sf::Mouse::Left) && fun::Input::Hold(sf::Keyboard::LControl)) view.move(fun::Input::M2D() * (view.getSize() / (sf::Vector2f)window.getSize()) * sf::Vector2f(-1, -1));

        graph.Update();

        ImGui::Begin("Filters", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
            if (ImGui::Button("Open Image")) {
                is_file_loaded = original_image.loadFromFile(open_file_dir);

                if (is_file_loaded) {
                    image = original_image;
                }
            }

            ImGui::SameLine();
            ImGui::InputText("##OpenImageBtn", open_file_dir, sizeof open_file_dir);
            ImGui::NewLine();
            if (ImGui::SliderFloat("Brightness", &brightness_scale, 0.f, 3.f)) should_update = true;
            if (ImGui::SliderInt("Sharpen", &sharpen_scale, 0, 5)) should_update = true;
            ImGui::NewLine();
            if (ImGui::ColorPicker3("MulCol", (float*)&mul_col, ImGuiColorEditFlags_None)) should_update = true;
            ImGui::NewLine();
            if (ImGui::Checkbox("Greyscale", &greyscale)) should_update = true;
            ImGui::NewLine();

            should_update = ImGui::Button("Update") || (should_update && auto_update);

            ImGui::SameLine();
            ImGui::Checkbox("Auto Update", &auto_update);

            if (is_file_loaded) {
                ImGui::NewLine();

                if (ImGui::Button("Save As")) {
                    image.saveToFile(out_file_dir);
                }

                ImGui::SameLine();
                ImGui::InputText("##SaveImageBtn", out_file_dir, sizeof out_file_dir);
                ImGui::NewLine();
            }
        ImGui::End();

        if (should_update && is_file_loaded){
            should_update = false;

            for (int i = 0; i < image.getSize().x; i++) {
                for (int j = 0; j < image.getSize().y; j++) {
                    Col c = (Col)original_image.getPixel(i, j);

                    c = c.pow(sharpen_scale);
                    c = c * mul_col;
                    c = c * brightness_scale;

                    if (greyscale) c.r = c.g = c.b = (c.r + c.g + c.b) * 0.333f;

                    image.setPixel(i, j, (sf::Color)c);
                }
            }

            texture.loadFromImage(image);
            sprite.setTexture(texture, true);
        }

        //ImGui::ShowDemoWindow();

        window.clear(sf::Color(44, 44, 44));
        window.setView(view);
        if (is_file_loaded) window.draw(sprite);
        window.draw(graph);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}