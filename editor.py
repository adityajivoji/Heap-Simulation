# import pygame
# import cv2 as cv
# import random
# from pygame.constants import K_ESCAPE
# pygame.init()
# clock = pygame.time.Clock()
# start_pos_x = 320
# start_pos_y = 240

# screen = pygame.display.set_mode((640,480))
# head = pygame.image.load('3.jpg').convert()
# head_rect = head.get_rect(center = (320,240))
# body = pygame.image.load('2.jpg').convert()
# body_rect = body.get_rect(center = (335,240))
# food = pygame.image.load('1.jpg').convert()
# food_rect = food.get_rect(center = (300,300))
# bg = pygame.image.load('grass.jpg')
# position = [(start_pos_x,start_pos_y)]
# rectangles = [head_rect]
# movement = previous_movement = 4
# counter = 0
# condition = random.randint(1,50)
# display_condition = False
# exit_condition = True
# while exit_condition:
#     screen.blit(bg,(0,0))
#     # If sanke ate the food
#     if head_rect.colliderect(food_rect):
#         position.append(position[length-1])
#         rectangles.append(body.get_rect(center = position[length - 1]))
#         display_condition = False
#         condition = condition + random.randint(1,25)
        
#     # Food Generation
#     if counter == condition:
#         food_rect.x = random.randint(1,639)
#         food_rect.y = random.randint(1,480)
#         condition = random.randint(50,80)
#         counter = 0
#         display_condition = True
#     # checking if 
#     length = len(position)
#     limit = length - 1
#     if(limit >= 1):
#         for i in range(limit):
#             position[limit - i] = position[limit - i - 1]
#     if(previous_movement - movement == 2):
#         movement == previous_movement
#     if movement == 0:
#         position[0] = (position[0][0] + 15 , position[0][1])
#     elif movement == 1:
#         position[0] = (position[0][0], position[0][1] + 15)
#     elif movement == 2:
#         position[0] = (position[0][0] - 15 , position[0][1])
#     elif movement == 3:
#         position[0] = (position[0][0], position[0][1] - 15)
    
#     for i in range(length):
#         rectangles[i].x = position[i][0]
#         rectangles[i].y = position[i][1]
    
#     if display_condition and counter < random.randint(45, 55):
#         screen.blit(food, food_rect)
    
#     for rect in rectangles:
#         if(rect == head_rect):
#             screen.blit(head,rect)
#         else:
#             screen.blit(body,rect)
#     counter = counter + 1
#     pygame.display.update()

#     for events in pygame.event.get():
#         if events.type == pygame.QUIT:
#             exit_condition = False
#         elif events.type == pygame.KEYDOWN:
#             previous_movement = movement
#             if(events.key == pygame.K_UP):
#                 movement = 3
#             elif(events.key == pygame.K_DOWN):
#                 movement = 1
#             elif(events.key == pygame.K_RIGHT):
#                 movement = 0
#             elif(events.key == pygame.K_LEFT):
#                 movement = 2
#             elif(events.key == K_ESCAPE):
#                 exit_condition == False
#     clock.tick(10)



































import cv2 as cv

img = cv.imread("grass.jpg")
print(img.shape)
cv.imshow("image",img)
im = img[699:1299,699:1299]
cv.imwrite("grass.jpg",im)
cv.waitKey()