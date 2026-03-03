#include "team2.h"


//shark

//stock market
constexpr int STOCK_HISTORY_LENGTH = 125;
struct stockMarketState {
    int funds{};
    int time_step{};
    int stock1[STOCK_HISTORY_LENGTH]{};
    int stock1Trend{};
    int stock2[STOCK_HISTORY_LENGTH]{};
    int stock2Trend{};
    int stock3[STOCK_HISTORY_LENGTH]{};
    int stock3Trend{};
    int doubleCounter{};
    int ownedStock1{};
    int ownedStock2{};
    int ownedStock3{};
};

union ezAutoCasting {
    void * in;
    stockMarketState * stock;
};

void * stockMarketCreate() {
    void * state = malloc(sizeof(stockMarketState));
    ezAutoCasting ez{};
    ez.in = state;
    for (int i=0;i<STOCK_HISTORY_LENGTH;i++) {
        ez.stock->stock1[i] = 0;
        ez.stock->stock2[i] = 0;
        ez.stock->stock3[i] = 0;
    }
    ez.stock->time_step = 0;
    ez.stock->stock1Trend = 0;
    ez.stock->stock2Trend = 0;
    ez.stock->stock3Trend = 0;
    ez.stock->stock1[0] = GetRandomValue(50,250);
    ez.stock->stock2[0] = GetRandomValue(50,250);
    ez.stock->stock3[0] = GetRandomValue(50,250);
    ez.stock->doubleCounter = 0;
    ez.stock->ownedStock1=0;
    ez.stock->ownedStock2=0;
    ez.stock->ownedStock3=0;
    return state;
}

void processStockValue(int timeStep, int* stock,int& trend,int volatility) {
    if (timeStep < STOCK_HISTORY_LENGTH) {
        int currentVal = GetRandomValue(50,250);
        if (timeStep!=0) {
            currentVal = stock[timeStep-1];
        }
        stock[timeStep] = GetRandomValue((int)std::max(0,currentVal-volatility+trend),(int)std::min(300,currentVal+volatility+trend));
        //compute the trend
        if (timeStep>=1) {
            trend = (stock[timeStep] - stock[timeStep-1])/3;
        }
        if (timeStep>=2) {
            trend += (stock[timeStep-1]-stock[timeStep-2])/3;
        }
    } else {
        //bubble each elemnt to the left
        for (int i=0;i<STOCK_HISTORY_LENGTH-1;i++) {
            stock[i] = stock[i+1];
        }
        int currentVal = stock[STOCK_HISTORY_LENGTH-1];
        stock[STOCK_HISTORY_LENGTH-1] = GetRandomValue((int)std::max(0,currentVal-volatility+trend),(int)std::min(300,currentVal+volatility+trend));
        // compute the trend
        trend = (stock[STOCK_HISTORY_LENGTH-1] - stock[STOCK_HISTORY_LENGTH-2])/3 + (stock[STOCK_HISTORY_LENGTH-2]- stock[STOCK_HISTORY_LENGTH-3])/3;
    }
}

void drawSimpleButton(int x, int y, int width,int height, std::string text, Color color) {
    DrawRectangle(x,y,width,height,color);
    const int textWidth = MeasureText(text.c_str(),30);//calculate how long the text on the button is
    const int textRight = x + width/2 - textWidth/2;//calculate the right x coord of the text
    DrawText(text.c_str(),textRight,y+(height-30)/2,30,BLACK);//render the text on the button
}

void stockMarketDraw(void * state, int x, int y) {
    auto * s = static_cast<stockMarketState*>(state);
    DrawRectangle(x,y,500,500,BLACK);
    std::string fundsText = "Funds: $";
    fundsText += std::to_string(s->funds);
    if (s->funds < 0) {
        DrawText(fundsText.c_str(),x+10,y+20,30,RED);
    } else {
        DrawText(fundsText.c_str(),x+10,y+20,30,GREEN);
    }
    int lineLength = 500/STOCK_HISTORY_LENGTH;
    for (int i=0;i<STOCK_HISTORY_LENGTH-1;i++) {
        DrawLine(x+i*lineLength,y+400-s->stock1[i],x+i*lineLength+lineLength,y+400-s->stock1[i+1],RED);
        DrawLine(x+i*lineLength,y+400-s->stock2[i],x+i*lineLength+lineLength,y+400-s->stock2[i+1],ORANGE);
        DrawLine(x+i*lineLength,y+400-s->stock3[i],x+i*lineLength+lineLength,y+400-s->stock3[i+1],BLUE);
    }

    DrawRectangle(x,y+95,500,2,GRAY);
    DrawRectangle(x,y+400,500,2,GRAY);

    s->doubleCounter++;
    if (s->doubleCounter%8==0) {
        s-> time_step ++;
        processStockValue(s->time_step,s->stock1,s->stock1Trend,4);
        processStockValue(s->time_step,s->stock2,s->stock2Trend,6);
        processStockValue(s->time_step,s->stock3,s->stock3Trend,8);
    }

    //draw the buttons
    drawSimpleButton(6+x,455+y,70,40,"Buy",RED);
    drawSimpleButton(6+83+x,455+y,70,40,"Sell",RED);
    drawSimpleButton(6+83*2+x,455+y,70,40,"Buy",ORANGE);
    drawSimpleButton(6+83*3+x,455+y,70,40,"Sell",ORANGE);
    drawSimpleButton(6+83*4+x,455+y,70,40,"Buy",BLUE);
    drawSimpleButton(6+83*5+x,455+y,70,40,"Sell",BLUE);

    std::string stock1Level = std::to_string(s->ownedStock1);
    std::string stock2Level = std::to_string(s->ownedStock2);
    std::string stock3Level = std::to_string(s->ownedStock3);

    DrawText(stock1Level.c_str(),x+10,y+420,30,RED);
    DrawText(stock2Level.c_str(),x+180,y+420,30,ORANGE);
    DrawText(stock3Level.c_str(),x+350,y+420,30,BLUE);

}

void stockMarketClicked(void * state, int button, int mouseX, int mouseY) {
    // drawSimpleButton(6+x,455+y,70,40,"Buy",RED);
    // drawSimpleButton(6+83+x,455+y,70,40,"Sell",RED);
    // drawSimpleButton(6+83*2+x,455+y,70,40,"Buy",ORANGE);
    // drawSimpleButton(6+83*3+x,455+y,70,40,"Sell",ORANGE);
    // drawSimpleButton(6+83*4+x,455+y,70,40,"Buy",BLUE);
    // drawSimpleButton(6+83*5+x,455+y,70,40,"Sell",BLUE);
}

bool stockMarkeySuccess(void * state) {
    return true;
}

std::string stockMarketGetInstructions(void * state) {
    return "Make $500 in the socket market";
}

//harm the huma



//teacton time



//this should be at the bottom
std::vector<captchaInfo> team2_get_captchas() {
    return {
        //shark

        //stock market
        {
            "stock_market",
            500, 500,
            &stockMarketCreate, // create function
            &stockMarketDraw, // draw function
            &stockMarketClicked, //mouse clicked function
            &stockMarkeySuccess, // success function
            &stockMarketGetInstructions,//instructions function
        }

        //harm the huma



        //teacton time


    };
}