/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

Game::Game(MainWindow& wnd)
	:
	wnd(wnd),
	gfx(wnd),
	x(30), y(40),
	color(Colors::Green),
	oldColor(color),
	speed(8),
	isColorChanged(false),
	frameX(gfx.ScreenWidth  / 4),
	frameY(gfx.ScreenHeight / 4),
	frameW(gfx.ScreenWidth  / 2),
	frameH(gfx.ScreenHeight / 2),
	x1(100), y1(100),
	d(Direction::RIGHT)
{
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::handleSpeed() {
	if (wnd.kbd.KeyIsPressed(VK_NUMPAD1)) {
		speed = 1;
	}
	if (wnd.kbd.KeyIsPressed(VK_NUMPAD2)) {
		speed = 4;
	}
	if (wnd.kbd.KeyIsPressed(VK_NUMPAD3)) {
		speed = 8;
	}
}

void Game::handleColorUpdate() {
	if (isColorChanged)
		return;

	if (wnd.kbd.KeyIsPressed('R')) {
		color.SetR((color.GetR() + 1) % 255);
	}
	if (wnd.kbd.KeyIsPressed('G')) {
		color.SetG((color.GetG() + 1) % 255);
	}
	if (wnd.kbd.KeyIsPressed('B')) {
		color.SetB((color.GetB() + 1) % 255);
	}

	oldColor = color;
}

void Game::handleObjectMovement() {
	if (wnd.kbd.KeyIsPressed(VK_RIGHT)) {
		x += speed;
	}

	if (wnd.kbd.KeyIsPressed(VK_LEFT)) {
		x -= speed;
	}

	if (wnd.kbd.KeyIsPressed(VK_UP)) {
		y -= speed;
	}

	if (wnd.kbd.KeyIsPressed(VK_DOWN)) {
		y += speed;
	}
}

void Game::handleCentralFrameMovement() {
	if (wnd.kbd.KeyIsPressed('D')) {
		frameX += speed;
	}

	if (wnd.kbd.KeyIsPressed('A')) {
		frameX -= speed;
	}

	if (wnd.kbd.KeyIsPressed('W')) {
		frameY -= speed;
	}

	if (wnd.kbd.KeyIsPressed('S')) {
		frameY += speed;
	}
}

void Game::handleColorCenterFrame() {
	if (!isColorChanged && 
		isCursorInsideCentralFrame()) {
		isColorChanged = true;
		oldColor = color;
		color = Colors::Red;
	}
	else if(!isCursorInsideCentralFrame()){
		isColorChanged = false;
		color = oldColor;
	}
}

void Game::handleCrosshairMargins() {
	if (x < 5) {
		x = 5;
	}
	if (x >= gfx.ScreenWidth - 6) {
		x = gfx.ScreenWidth - 6;
	}
	if (y < 5) {
		y = 5;
	}
	if (y >= gfx.ScreenHeight - 6) {
		y = gfx.ScreenHeight - 6;
	}
}

void Game::handleCentralFrameMargins() {
	if (frameX <= 5) {
		frameX = 5;
	}
	if (frameX + frameW >= gfx.ScreenWidth - 5) {
		frameX = gfx.ScreenWidth - frameW - 5;
	}
	if (frameY <= 5) {
		frameY = 5;
	}
	if (frameY + frameH >= gfx.ScreenHeight - 5) {
		frameY = gfx.ScreenHeight - frameH - 5;
	}
}

void Game::UpdateModel()
{
	handleSpeed();
	handleColorUpdate();
	handleColorCenterFrame();
	handleObjectMovement();
	handleCentralFrameMovement();
	handleCrosshairMargins();
	handleCentralFrameMargins();
}

void Game::drawCentralFrame() {
	for (int i = frameX; i < frameX + frameW; i++) {
		gfx.PutPixel(i, frameY,			 Colors::Gray);
		gfx.PutPixel(i, frameY + frameH, Colors::Gray);
	}
	for (int i = frameY; i < frameY + frameH; i++) {
		gfx.PutPixel(frameX,		  i, Colors::Gray);
		gfx.PutPixel(frameX + frameW, i, Colors::Gray);
	}
}

void Game::drawCrosshair(const int x, const int y, const int size) {
	
	for (int i = x - size / 2; i <= x + size / 2; i++) {
		gfx.PutPixel(i, y, color);
	}
	
	for (int i = y - size / 2; i <= y + size / 2; i++) {
		gfx.PutPixel(x, i, color);
	}

	//gfx.PutPixel(x - 5, y	 , color);
	//gfx.PutPixel(x - 4, y	 , color);
	//gfx.PutPixel(x - 3, y	 , color);
	//gfx.PutPixel(x + 3, y	 , color);
	//gfx.PutPixel(x + 4, y	 , color);
	//gfx.PutPixel(x + 5, y	 , color);
	//gfx.PutPixel(x	  , y - 5, color);
	//gfx.PutPixel(x	  , y - 4, color);
	//gfx.PutPixel(x	  , y - 3, color);
	//gfx.PutPixel(x	  , y + 3, color);
	//gfx.PutPixel(x	  , y + 4, color);
	//gfx.PutPixel(x	  , y + 5, color);
}

void Game::moveAutoMovingCrosshair()
{
	// update position
	// move - right, down, left, up
	//
	// (30,30)
	//			---->
	//      +---------------+
	//	^	|				|  |
	//	|	|				|  |
	//	|	|				|  ? 
	//      +---------------+
	//			<----		(w-30, h-30)

	switch (d) {
	case Direction::LEFT:
		if (100 <= x1 && x1 <= gfx.ScreenWidth - 100)
			x1 -= speed;
		else if (x1 <= 100) {
			x1 = 100;
			d = Direction::UP;
		}
		break;
	case Direction::RIGHT:
		if (100 <= x1 && x1 < gfx.ScreenWidth - 100)
			x1 += speed;
		else if (x1 >= gfx.ScreenWidth - 100) {
			x1 = gfx.ScreenWidth - 100;
			d = Direction::DOWN;
		}
		break;
	case Direction::UP:
		if (100 <= y1 && y1 <= gfx.ScreenHeight - 100)
			y1 -= speed;
		else if (y1 <= 100) {
			y1 = 100;
			d = Direction::RIGHT;
		}
		break;
	case Direction::DOWN:
		if (100 <= y1 && y1 < gfx.ScreenHeight - 100)
			y1 += speed;
		else if (y1 >= gfx.ScreenHeight - 100) {
			y1 = gfx.ScreenHeight - 100;
			d = Direction::LEFT;
		}
		break;

	}
}

void Game::ComposeFrame()
{
	drawCentralFrame();
	drawCrosshair(x, y, 11);

	// self moving crosshair
	drawCrosshair(x1, y1, 11);

	moveAutoMovingCrosshair();
}
