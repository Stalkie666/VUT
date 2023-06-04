/*!
 * @file
 * @brief This file contains implementation of gpu
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#include <student/gpu.hpp>

#include <iostream>
#include <fstream>

int drawID_Global = 0;

void clear( GPUMemory &mem,ClearCommand cmd  ){
  if(cmd.clearColor){
    int realSize = mem.framebuffer.height * mem.framebuffer.width * 4;
    for( int i = 0; (i + 3) < realSize; i += 4 ){
      float red = cmd.color.r;
      mem.framebuffer.color[i+0] = (uint8_t)(red * 255.f);
      float green = cmd.color.g;
      mem.framebuffer.color[i+1] = (uint8_t)(green * 255.f);
      float blue = cmd.color.b;
      mem.framebuffer.color[i+2] = (uint8_t)(blue * 255.f);
      float alpha = cmd.color.a;
      mem.framebuffer.color[i+3] = (uint8_t)(alpha * 255.f);
    }
  }
  if( cmd.clearDepth ){
    int realSize = mem.framebuffer.height * mem.framebuffer.width;
    for(int i = 0; i < realSize; ++i){
      mem.framebuffer.depth[i] = cmd.depth;
    }
  }
}

uint32_t computeVertexID( GPUMemory & mem, VertexArray const&vao,uint32_t shaderIncocation){
  if(vao.indexBufferID < 0) return shaderIncocation;
 
  Buffer indexBuffer = mem.buffers[vao.indexBufferID];
  
  uint64_t offsetTmp = vao.indexOffset / 2;

  if( vao.indexType == IndexType::UINT32){
    uint32_t*ind = ( ((uint32_t*)indexBuffer.data) + offsetTmp);
    return ind[shaderIncocation];
  }
  else if(vao.indexType == IndexType::UINT16){
    uint16_t*ind = ( ((uint16_t*)indexBuffer.data) + offsetTmp);
    return ind[shaderIncocation];
  }
  else if(vao.indexType == IndexType::UINT8){
    uint8_t*ind = ( ((uint8_t*)indexBuffer.data) + offsetTmp);
    return ind[shaderIncocation];
  }
  else
    return 0;
}

InVertex vertexAssembly(GPUMemory &mem,VertexArray vao,InVertex invertex){
  InVertex returnVertex = invertex;
  for(uint32_t i = 0; i < maxAttributes;++i){
    uint64_t offsetTmp = vao.vertexAttrib[i].offset;
    uint64_t strideTmp = vao.vertexAttrib[i].stride;

  void * Buffer = (void*) (mem.buffers[vao.vertexAttrib[i].bufferID].data);
  char * attributeAddress = (char*)Buffer + offsetTmp + invertex.gl_VertexID*strideTmp; 

    switch( vao.vertexAttrib[i].type ){
      case AttributeType::FLOAT:
        returnVertex.attributes[i].v1 = *((float*)attributeAddress);
        break;
      case AttributeType::VEC2:
        returnVertex.attributes[i].v2 = *((glm::vec2*)attributeAddress);
        break;
      case AttributeType::VEC3:
        returnVertex.attributes[i].v3 = *((glm::vec3*)attributeAddress);
        break;
      case AttributeType::VEC4:
      returnVertex.attributes[i].v4 = *((glm::vec4*)attributeAddress);
      break;
    }
  }
  return returnVertex;
}

struct Triangle{
  OutVertex points[3];
};

void perspectiveDivision(Triangle & triangle){
  for(int i = 0; i < 3; ++i){
    float W = triangle.points[i].gl_Position.w;
    triangle.points[i].gl_Position.x /= W;
    triangle.points[i].gl_Position.y /= W;
    triangle.points[i].gl_Position.z /= W; 
  }
}

void viewportTransformation(Triangle & triangle,uint32_t width, uint32_t height){
  for(int32_t i = 0; i < 3; ++i){
    triangle.points[i].gl_Position.x += 1.f;
    triangle.points[i].gl_Position.y += 1.f;

    triangle.points[i].gl_Position.x /= 2.f;
    triangle.points[i].gl_Position.y /= 2.f;

    triangle.points[i].gl_Position.x *= width;
    triangle.points[i].gl_Position.y *= height;

    triangle.points[i].gl_Position.x -= 1;
    triangle.points[i].gl_Position.y -= 1;
  }
}

struct Point{
  float x,y;
};

float calculateArea(Point A, Point B, Point C){
  glm::mat3 matice = glm::mat3( A.x,A.y,1,
                                B.x,B.y,1,
                                C.x,C.y,1);
  float determinant = glm::determinant(matice);
  determinant /= 2.f;
  return fabs(determinant);
}

float calculateDepth(Triangle&triangle,Point point){
  Point points[3];
  for(int i = 0; i < 3; ++i){
    points[i].x = triangle.points[i].gl_Position.x;
    points[i].y = triangle.points[i].gl_Position.y;
  }
  float lambda[3];
  lambda[0] = calculateArea(points[1],points[2],point) / calculateArea(points[0],points[1],points[2]);
  lambda[1] = calculateArea(points[0],points[2],point) / calculateArea(points[0],points[1],points[2]);
  lambda[2] = calculateArea(points[1],points[0],point) / calculateArea(points[0],points[1],points[2]);

  float hloubka = 0;
  hloubka += triangle.points[0].gl_Position.z * lambda[0];
  hloubka += triangle.points[1].gl_Position.z * lambda[1];
  hloubka += triangle.points[2].gl_Position.z * lambda[2];
  return hloubka;
}

glm::vec4 calculateColor(Triangle&triangle,Point point){
  float R,G,B,A;

  Point points[3];
  for(int i = 0; i < 3; ++i){
    points[i].x = triangle.points[i].gl_Position.x + 0.5;
    points[i].y = triangle.points[i].gl_Position.y + 0.5;
  }
  float lambda2D[3];
  lambda2D[0] = calculateArea(points[1],points[2],point) / calculateArea(points[0],points[1],points[2]);
  lambda2D[1] = calculateArea(points[0],points[2],point) / calculateArea(points[0],points[1],points[2]);
  lambda2D[2] = calculateArea(points[1],points[0],point) / calculateArea(points[0],points[1],points[2]);

  float H[3];
  for(int i = 0; i < 3;++i)
    H[i] = triangle.points[i].gl_Position.w;

  float S = lambda2D[0] / H[0]
          + lambda2D[1] / H[1]
          + lambda2D[2] / H[2];

  float lambda[3];
  for(int i = 0; i < 3; ++i)
    lambda[i] = lambda2D[i] / (S * H[i]);


  R = 0;
  R += lambda[0] * triangle.points[0].attributes[0].v3.r;
  R += lambda[1] * triangle.points[1].attributes[0].v3.r;
  R += lambda[2] * triangle.points[2].attributes[0].v3.r;

  G = 0;
  G += lambda[0] * triangle.points[0].attributes[0].v3.g;
  G += lambda[1] * triangle.points[1].attributes[0].v3.g;
  G += lambda[2] * triangle.points[2].attributes[0].v3.g;

  B = 0;
  B += lambda[0] * triangle.points[0].attributes[0].v3.b;
  B += lambda[1] * triangle.points[1].attributes[0].v3.b;
  B += lambda[2] * triangle.points[2].attributes[0].v3.b;

  A = 0;
  A += lambda[0] * triangle.points[0].attributes[0].v4.a;
  A += lambda[1] * triangle.points[1].attributes[0].v4.a;
  A += lambda[2] * triangle.points[2].attributes[0].v4.a;

  return glm::vec4(R,G,B,A);
}

void rasterize(Frame&frame,Triangle &triangle,Program const&prg, bool backFaceCulling){
  // kontrola jestli jsme po smerunebo protismeru
  glm::vec3 v1 = glm::vec3( triangle.points[1].gl_Position.x - triangle.points[0].gl_Position.x,
                            triangle.points[1].gl_Position.y - triangle.points[0].gl_Position.y, 1);
  glm::vec3 v2 = glm::vec3( triangle.points[2].gl_Position.x - triangle.points[1].gl_Position.x,
                            triangle.points[2].gl_Position.y - triangle.points[1].gl_Position.y, 1);
  glm::vec3 v3 = glm::vec3( triangle.points[0].gl_Position.x - triangle.points[2].gl_Position.x,
                            triangle.points[0].gl_Position.y - triangle.points[2].gl_Position.y, 1);
  

  //glm::mat3 matice = glm::mat3( v1,v2,v3 );
  if( glm::determinant( glm::mat3( v1,v2,v3 ) ) < 0 ){
    if( backFaceCulling == false){
      OutVertex tmp = triangle.points[1];
      triangle.points[1] = triangle.points[2];
      triangle.points[2] = tmp;
    }
    else
      return;
  }
 // hranice trojuhelniku - vypocty
  int minX = frame.width;
  int minY = frame.height;
  int maxX = 0;
  int maxY = 0;
  for( int32_t i = 0; i < 3; ++i ){
    if( triangle.points[i].gl_Position.x < minX ) minX = triangle.points[i].gl_Position.x;
    if( triangle.points[i].gl_Position.y < minY ) minY = triangle.points[i].gl_Position.y;
    if( triangle.points[i].gl_Position.x > maxX ) maxX = triangle.points[i].gl_Position.x;
    if( triangle.points[i].gl_Position.y > maxY ) maxY = triangle.points[i].gl_Position.y;
  }
  maxX = ( maxX > frame.width  ) ? frame.width : maxX;
  maxY = ( maxY > frame.height ) ? frame.height : maxY;
  minX = ( minX < 0 ) ? 0 : minX;
  minY = ( minY < 0 ) ? 0 : minY;

  int h1x = triangle.points[1].gl_Position.x - triangle.points[0].gl_Position.x;
  int h2x = triangle.points[2].gl_Position.x - triangle.points[1].gl_Position.x;
  int h3x = triangle.points[0].gl_Position.x - triangle.points[2].gl_Position.x;

  int h1y = triangle.points[1].gl_Position.y - triangle.points[0].gl_Position.y;
  int h2y = triangle.points[2].gl_Position.y - triangle.points[1].gl_Position.y; 
  int h3y = triangle.points[0].gl_Position.y - triangle.points[2].gl_Position.y; 

  int b1x = minX - triangle.points[0].gl_Position.x;
  int b1y = minY - triangle.points[0].gl_Position.y;
  int b2x = minX - triangle.points[1].gl_Position.x;
  int b2y = minY - triangle.points[1].gl_Position.y;
  int b3x = minX - triangle.points[2].gl_Position.x;
  int b3y = minY - triangle.points[2].gl_Position.y;

  int e1 = b1y * h1x - b1x * h1y;
  int e2 = b2y * h2x - b2x * h2y;
  int e3 = b3y * h3x - b3x * h3y;


  for(int y = minY; y < maxY; ++y){
    int t1 = e1;
    int t2 = e2;
    int t3 = e3;
    for( int x = minX; x < maxX; ++x ){
      if( t1 >= 0 && t2 >= 0 && t3 >= 0 ){
        InFragment infragment;
        OutFragment outFragment;
        ShaderInterface si;

        infragment.gl_FragCoord.x = x + 0.5f;
        infragment.gl_FragCoord.y = y + 0.5f;

        Point tmp;
        tmp.x = x;
        tmp.y = y;
        infragment.gl_FragCoord.z = calculateDepth(triangle,tmp);
        infragment.attributes[0].v4 = calculateColor(triangle,tmp);
        prg.fragmentShader(outFragment,infragment,si);

        //zmena buferu
        int32_t position = y*frame.width + x;
        float alpha = outFragment.gl_FragColor.a;
        if( alpha < 1.f ){
          frame.color[position*4 + 0] =(uint8_t)(glm::clamp((frame.color[position*4 + 0] / 255.f)*(1-alpha)+(outFragment.gl_FragColor.r)*(alpha),0.f,1.f) * 255.f );
          frame.color[position*4 + 1] =(uint8_t)(glm::clamp((frame.color[position*4 + 1] / 255.f)*(1-alpha)+(outFragment.gl_FragColor.g)*(alpha),0.f,1.f) * 255.f );
          frame.color[position*4 + 2] =(uint8_t)(glm::clamp((frame.color[position*4 + 2] / 255.f)*(1-alpha)+(outFragment.gl_FragColor.b)*(alpha),0.f,1.f) * 255.f + 0.000005f );
        }
        if( frame.depth[x*y] >= infragment.gl_FragCoord.z ){ 
          if( alpha == 1 ){
            frame.color[position*4 + 0] = (uint8_t)( outFragment.gl_FragColor.r * 255.f);
            frame.color[position*4 + 1] = (uint8_t)( outFragment.gl_FragColor.g * 255.f);
            frame.color[position*4 + 2] = (uint8_t)( outFragment.gl_FragColor.b * 255.f);
          }
          frame.color[position*4 + 3] = (uint8_t)( outFragment.gl_FragColor.a * 255.f);
          if(alpha > 0.5)
            frame.depth[position] = infragment.gl_FragCoord.z;
        }
      }
      t1 -= h1y;
      t2 -= h2y;
      t3 -= h3y;
    }
    e1 += h1x;
    e2 += h2x;
    e3 += h3x;
  }


  
}

void draw(GPUMemory &mem, DrawCommand cmd){
  VertexShader vs = mem.programs[cmd.programID].vertexShader;
  std::vector <OutVertex> outVertexArray;
  outVertexArray.clear();
  for( uint32_t n = 0; n < cmd.nofVertices; ++n ){
    InVertex inVertex;
    OutVertex outVertex;
    ShaderInterface si;
    inVertex.gl_VertexID = computeVertexID(mem,cmd.vao,n); //mozna neco jineho
    inVertex.gl_DrawID = drawID_Global;
    inVertex = vertexAssembly(mem,cmd.vao,inVertex);
    vs(outVertex,inVertex,si);
    outVertexArray.push_back(outVertex);
  }


  for( uint32_t t = 0; t+2 < cmd.nofVertices; t += 3 ){
    Triangle triangle;
    triangle.points[0] = outVertexArray[t];
    triangle.points[1] = outVertexArray[t+1];
    triangle.points[2] = outVertexArray[t+2];

    perspectiveDivision(triangle);
    viewportTransformation(triangle,mem.framebuffer.width,mem.framebuffer.height);

    rasterize(mem.framebuffer,triangle,mem.programs[cmd.programID],cmd.backfaceCulling);

    
  }


}

//! [gpu_execute]
void gpu_execute(GPUMemory&mem,CommandBuffer &cb){
  (void)mem;
  (void)cb;
  /// \todo Tato funkce reprezentuje funkcionalitu grafické karty.<br>
  /// Měla by umět zpracovat command buffer, čistit framebuffer a kresli.<br>
  /// mem obsahuje paměť grafické karty.
  /// cb obsahuje command buffer pro zpracování.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
  drawID_Global = 0;
  
  for( uint32_t i = 0; i < cb.nofCommands; ++i ){
    CommandType type = cb.commands[i].type;
    CommandData data = cb.commands[i].data;
    if( type == CommandType::CLEAR ){
      clear(mem, data.clearCommand);    
    }
    else if( type == CommandType::DRAW ){
      draw(mem,data.drawCommand);
      drawID_Global++;
    }    
  }
  
}
//! [gpu_execute]

/**
 * @brief This function reads color from texture.
 *
 * @param texture texture
 * @param uv uv coordinates
 *
 * @return color 4 floats
 */
glm::vec4 read_texture(Texture const&texture,glm::vec2 uv){
  if(!texture.data)return glm::vec4(0.f);
  auto uv1 = glm::fract(uv);
  auto uv2 = uv1*glm::vec2(texture.width-1,texture.height-1)+0.5f;
  auto pix = glm::uvec2(uv2);
  //auto t   = glm::fract(uv2);
  glm::vec4 color = glm::vec4(0.f,0.f,0.f,1.f);
  for(uint32_t c=0;c<texture.channels;++c)
    color[c] = texture.data[(pix.y*texture.width+pix.x)*texture.channels+c]/255.f;
  return color;
}

