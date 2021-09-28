import pygame
import cv2 as cv
import random
#from pygame.constants import K_ESCAPE
pygame.init()
clock = pygame.time.Clock()
start_pos_x = 320
start_pos_y = 240
screen = pygame.display.set_mode((600,700))
head = pygame.image.load('3.jpg').convert_alpha()
head_rect = head.get_rect(center = (320,240))
body = pygame.image.load('2.jpg').convert_alpha()
body_rect = body.get_rect(center = (335,240))
food = pygame.image.load('1.jpg').convert_alpha()
food_rect = food.get_rect(center = (300,300))
position = [(start_pos_x,start_pos_y)]
rectangles = [head_rect]
movement = previous_movement = 33
counter = 0
condition = random.randint(1,50)
display_condition = False
exit_condition = True
grab_time = random.randint(150, 555)
rect_screen = pygame.Rect(0, 0, 600, 100)
text_font = pygame.font.Font('comic.ttf',50)
text_rect = text_font.render('Snake Game',False,'Green')
text_font = pygame.font.Font('comic.ttf',25)
score = 0
score_rect = text_font.render(f'Score = {score}',False,'Green')
while exit_condition:

    # Disply background
    
    screen.fill((255,255,255))
    pygame.draw.rect(screen, (0,0,255), rect_screen)
    screen.blit(text_rect,(30,0))
    screen.blit(score_rect,(400,50))
    pygame.display.flip()
    # displaying food until its either eaten or the allowed time is finished
    if display_condition and counter < grab_time:
        screen.blit(food, food_rect)
    # Updating the postion array of the body
    length = len(position)
    limit = length - 1
    if(limit >= 1):
        for i in range(limit):
            position[limit - i] = position[limit - i - 1]
    # Updating the postion of the head
    
    if movement == 0:
        position[0] = (position[0][0] + 15 , position[0][1])
    elif movement == 1:
        position[0] = (position[0][0], position[0][1] + 15)
    elif movement == 2:
        position[0] = (position[0][0] - 15 , position[0][1])
    elif movement == 3:
        position[0] = (position[0][0], position[0][1] - 15)
    
    if( position[0][0] > 600):
        position[0] = (0 , position[0][1])
    elif position[0][0] < 0:
        position[0] = (600 , position[0][1])
    elif position[0][1] > 700:
        position[0] = (position[0][0] , 100)
    elif position[0][1] < 100:
        position[0] = (position[0][0] , 700)
    else:
        pass
    for i in range(length):
        rectangles[i].x = position[i][0]
        rectangles[i].y = position[i][1]
    
    
    
    for rect in rectangles:
        if(rect == head_rect):
            screen.blit(head,rect)
        else:
            screen.blit(body,rect)
    
        
    if display_condition and counter < grab_time and head_rect.colliderect(food_rect):
        position.append(position[length-1])
        rectangles.append(body.get_rect(center = position[length - 1]))
        display_condition = False
        condition = condition + random.randint(1,25)
        score = score + 1
        score_rect = text_font.render(f'Score = {score}',False,'Green')
        pygame.draw.rect(screen, (0,0,255), rect_screen)
        screen.blit(text_rect,(30,0))
        screen.blit(score_rect,(400,50))
        
    # Food Generation
    if counter == condition:
        food_rect.x = random.randint(1,600)
        food_rect.y = random.randint(100,700)
        condition = random.randint(50,80)
        counter = 0
        grab_time = random.randint(45, 55)
        display_condition = True
    for i in range (3,length):
        if head_rect.colliderect(rectangles[i]):
            screen.blit()
            exit_condition = False
    
    pygame.display.update()
    counter = counter + 1
    for events in pygame.event.get():
        if events.type == pygame.QUIT:
            exit_condition = False
        elif events.type == pygame.KEYDOWN:
            previous_movement = movement
            if(events.key == pygame.K_RIGHT):
                movement = 0
            elif(events.key == pygame.K_DOWN):
                movement = 1
            elif(events.key == pygame.K_LEFT):
                movement = 2
                print("right")
            elif(events.key == pygame.K_UP):
                movement = 3
            elif(events.key == pygame.K_ESCAPE):
                exit_condition == False
            if(abs(previous_movement - movement) == 2):
                movement = previous_movement
    clock.tick(15)