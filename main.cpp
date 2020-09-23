#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
constexpr int game_width = 200;
constexpr int game_height = 200;

class discord_art : public olc::PixelGameEngine
{
public:
	discord_art()
	{
		sAppName = "Discord Art Creator";
	}
                    //Name    R    G    B  
    olc::Pixel background_color {108, 108, 108},
               brown_square     {178,  84,  62},
               red_square       {210,  22,  52},
               blue_square      { 70, 154, 234},
               black_square     { 37,  41,  46},
               orange_square    {253, 156,  40},
               white_square     {224, 225, 226},
               green_square     {103, 165,  71},
               yellow_square    {252, 193,  71},
               purple_square    {153, 119, 204};

    //Preventing some magic numbers
    static constexpr uint8_t square_count = 9;
    static constexpr uint8_t square_dim = 8;
    static constexpr uint8_t width = 10;
    static constexpr uint8_t height = 10;
    static constexpr uint8_t offset_x = (game_width - width * square_dim) / 2;
    static constexpr uint8_t offset_y = (game_height - height * square_dim) / 2;
    static constexpr char const *const blank_tile = "      ";

    //Eraser
    std::pair<const olc::Pixel, const std::string> const blank_square{olc::BLANK, blank_tile};

    //All non-blank squares
    std::array<const std::pair<const olc::Pixel, const std::string>, square_count> const square_options{
                                                            std::make_pair(white_square,  ":white_large_square:"),
                                                            std::make_pair(black_square,  ":black_large_square:"),
                                                            std::make_pair(purple_square, ":purple_square:"),
                                                            std::make_pair(blue_square,   ":blue_square:"),
                                                            std::make_pair(red_square,    ":red_square:"),
                                                            std::make_pair(orange_square, ":orange_square:"),
                                                            std::make_pair(yellow_square, ":yellow_square:"),
                                                            std::make_pair(green_square,  ":green_square:"),
                                                            std::make_pair(brown_square,  ":brown_square:")};

    //Used to make life easier for output
    std::array<std::string, width * height> square_map;
    
    //Pointer to current square
    std::pair<const olc::Pixel, const std::string> const* current_square = &(square_options[0]);

public:
	bool OnUserCreate() override
	{
        //Fill array
        square_map.fill(blank_tile);
        
        //Set up background
        int background_layer = CreateLayer();
        SetDrawTarget(background_layer);
        Clear(background_color);

        DrawRect(offset_x - 1, offset_y - 1, width * square_dim + 1, height * square_dim + 1, olc::GREY);
        int square_options_offset = (ScreenWidth() - square_options.size() * square_dim) / 2;
        for(int i = 0; i < square_options.size(); ++i) {
           FillRect(square_options_offset + i * square_dim, 0, square_dim, square_dim, square_options[i].first);
           DrawString(square_options_offset + i * square_dim, 0, std::to_string(i), olc::BLACK);
        }
        //DO NOT REMOVE
        EnableLayer(background_layer, true);

        //Get ready to rumble
        SetDrawTarget(nullptr);
        Clear(olc::BLANK);

		return true;
	}
    
    void print_square_map() {
        printf("COPY FROM THE LINE BELOW THIS ONE ----\n");
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                printf("%s", square_map[x + y * width].c_str());
            }
            printf("\n");
        }
        printf(" ---- TO THIS ONE\n");
    }

	bool OnUserUpdate(float fElapsedTime) override
	{
        //Determine if square should swap
        for(int key = olc::Key::K0; key < square_options.size() + olc::Key::K0; ++key) {
            if(GetKey((olc::Key)key).bPressed) {
                current_square = &(square_options[key - olc::Key::K0]);
            }
        }

        //E for erase
        if(GetKey(olc::Key::E).bPressed) {
            current_square = &blank_square;
        }

        if(GetKey(olc::Key::SPACE).bPressed) {
            print_square_map();
        }

        //Drawing
        if(GetMouse(0).bHeld) {
            //Get relative tile position
            olc::vi2d relative_mouse_pos = (GetMousePos() - olc::vi2d(offset_x, offset_y)) / square_dim;
            
            if(relative_mouse_pos.x < width && relative_mouse_pos.y < height 
                && relative_mouse_pos.y >= 0 && relative_mouse_pos.x >= 0) {

                square_map[relative_mouse_pos.x + relative_mouse_pos.y * width] = current_square->second;
                FillRect(relative_mouse_pos.x * (square_dim) + offset_x, relative_mouse_pos.y * (square_dim) + offset_y, square_dim, square_dim, current_square->first);

            }
        }

		return true;
	}
};

int main()
{
	discord_art demo;
	if (demo.Construct(game_width, game_height, 4, 4))
		demo.Start();

	return 0;
}

