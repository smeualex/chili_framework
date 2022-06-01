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
	crosshairX(30), crosshairY(40),
	color(Colors::Green),
	oldColor(color),
	speed(8),
	isColorChanged(false),
	frameX(gfx.ScreenWidth  / 4),
	frameY(gfx.ScreenHeight / 4),
	frameW(gfx.ScreenWidth  / 2),
	frameH(gfx.ScreenHeight / 2),
	autoMovingCrosshairX(100), autoMovingCrosshairY(100),
	autoMovingCrosshairDirection(Direction::RIGHT)
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
		crosshairX += speed;
	}

	if (wnd.kbd.KeyIsPressed(VK_LEFT)) {
		crosshairX -= speed;
	}

	if (wnd.kbd.KeyIsPressed(VK_UP)) {
		crosshairY -= speed;
	}

	if (wnd.kbd.KeyIsPressed(VK_DOWN)) {
		crosshairY += speed;
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
	if (crosshairX < 5) {
		crosshairX = 5;
	}
	if (crosshairX >= gfx.ScreenWidth - 6) {
		crosshairX = gfx.ScreenWidth - 6;
	}
	if (crosshairY < 5) {
		crosshairY = 5;
	}
	if (crosshairY >= gfx.ScreenHeight - 6) {
		crosshairY = gfx.ScreenHeight - 6;
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

	for (int i = 3; i < size / 2; i++) {
		gfx.PutPixel(x - i, y,     color);
		gfx.PutPixel(x + i, y,	   color);
		gfx.PutPixel(x,		y - i, color);
		gfx.PutPixel(x,		y + i, color);
	}
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

	switch (autoMovingCrosshairDirection) {
	case Direction::LEFT:
		if (100 <= autoMovingCrosshairX && autoMovingCrosshairX <= gfx.ScreenWidth - 100)
			autoMovingCrosshairX -= speed;
		else if (autoMovingCrosshairX <= 100) {
			autoMovingCrosshairX = 100;
			autoMovingCrosshairDirection = Direction::UP;
		}
		break;
	case Direction::RIGHT:
		if (100 <= autoMovingCrosshairX && autoMovingCrosshairX < gfx.ScreenWidth - 100)
			autoMovingCrosshairX += speed;
		else if (autoMovingCrosshairX >= gfx.ScreenWidth - 100) {
			autoMovingCrosshairX = gfx.ScreenWidth - 100;
			autoMovingCrosshairDirection = Direction::DOWN;
		}
		break;
	case Direction::UP:
		if (100 <= autoMovingCrosshairY && autoMovingCrosshairY <= gfx.ScreenHeight - 100)
			autoMovingCrosshairY -= speed;
		else if (autoMovingCrosshairY <= 100) {
			autoMovingCrosshairY = 100;
			autoMovingCrosshairDirection = Direction::RIGHT;
		}
		break;
	case Direction::DOWN:
		if (100 <= autoMovingCrosshairY && autoMovingCrosshairY < gfx.ScreenHeight - 100)
			autoMovingCrosshairY += speed;
		else if (autoMovingCrosshairY >= gfx.ScreenHeight - 100) {
			autoMovingCrosshairY = gfx.ScreenHeight - 100;
			autoMovingCrosshairDirection = Direction::LEFT;
		}
		break;

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

	moveAutoMovingCrosshair();
}


void Game::ComposeFrame()
{
	drawCentralFrame();
	drawCrosshair(crosshairX, crosshairY, 11);
	// self moving crosshair
	drawCrosshair(autoMovingCrosshairX, autoMovingCrosshairY, 21);
}
