/*!
 * @file
 * @brief This file contains functions for model rendering
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */
#include <student/drawModel.hpp>
#include <student/gpu.hpp>

///\endcond

void addClearCommand(CommandBuffer & cb){
  int32_t index = cb.nofCommands;
  cb.commands[index].type = CommandType::CLEAR;
  cb.commands[index].data.clearCommand.clearColor = true;
  cb.commands[index].data.clearCommand.clearDepth = true;
  cb.commands[index].data.clearCommand.color = glm::vec4(0.1,0.15,0.1,1); 
  cb.commands[index].data.clearCommand.depth = 1e11;
  cb.nofCommands += 1;
}

void addDrawCommand(CommandBuffer & cb,Mesh const&mesh){
  int32_t index = cb.nofCommands;
  cb.commands[index].type = CommandType::DRAW;
  cb.commands[index].data.drawCommand.backfaceCulling = !mesh.doubleSided;
  cb.commands[index].data.drawCommand.programID = 0; 
  cb.commands[index].data.drawCommand.nofVertices = mesh.nofIndices;

  cb.commands[index].data.drawCommand.vao.indexType = mesh.indexType;
  cb.commands[index].data.drawCommand.vao.indexOffset = mesh.indexOffset;
  cb.commands[index].data.drawCommand.vao.indexBufferID = mesh.indexBufferID;

  //asi vsechny, ale bude to problem, mozna tam bude jiny pocet
  cb.commands[index].data.drawCommand.vao.vertexAttrib[0].bufferID  = mesh.position.bufferID;
  cb.commands[index].data.drawCommand.vao.vertexAttrib[0].offset    = mesh.position.offset;
  cb.commands[index].data.drawCommand.vao.vertexAttrib[0].stride    = mesh.position.stride;
  cb.commands[index].data.drawCommand.vao.vertexAttrib[0].type      = mesh.position.type;

  cb.commands[index].data.drawCommand.vao.vertexAttrib[1].bufferID  = mesh.normal.bufferID;
  cb.commands[index].data.drawCommand.vao.vertexAttrib[1].offset    = mesh.normal.offset;
  cb.commands[index].data.drawCommand.vao.vertexAttrib[1].stride    = mesh.normal.stride;
  cb.commands[index].data.drawCommand.vao.vertexAttrib[1].type      = mesh.normal.type;

  cb.commands[index].data.drawCommand.vao.vertexAttrib[2].bufferID  = mesh.texCoord.bufferID;
  cb.commands[index].data.drawCommand.vao.vertexAttrib[2].offset    = mesh.texCoord.offset;
  cb.commands[index].data.drawCommand.vao.vertexAttrib[2].stride    = mesh.texCoord.stride;
  cb.commands[index].data.drawCommand.vao.vertexAttrib[2].type      = mesh.texCoord.type;

  cb.nofCommands++;
}

void prepareNode(GPUMemory&mem,CommandBuffer&cb,Node const&node,Model const&model,glm::mat4 prubeznaMatice){
  if(node.mesh >= 0){
    Mesh const& mesh = model.meshes[node.mesh];
    addDrawCommand(cb,mesh);
  }
  
  prubeznaMatice = prubeznaMatice * node.modelMatrix;
  
  mem.uniforms[10 + (cb.nofCommands-2)*5+0].m4 = prubeznaMatice;
  mem.uniforms[10 + (cb.nofCommands-2)*5+1].m4 = glm::transpose(glm::inverse(prubeznaMatice));
  mem.uniforms[10 + (cb.nofCommands-2)*5+2].v4 = model.meshes[node.mesh].diffuseColor;
  mem.uniforms[10 + (cb.nofCommands-2)*5+3].i1 = model.meshes[node.mesh].diffuseTexture;
  mem.uniforms[10 + (cb.nofCommands-2)*5+4].v1 = model.meshes[node.mesh].doubleSided;
  

  for(size_t i = 0; i < node.children.size();++i)
    prepareNode(mem,cb,node.children[i],model,prubeznaMatice);
}


/**
 * @brief This function prepares model into memory and creates command buffer
 *
 * @param mem gpu memory
 * @param commandBuffer command buffer
 * @param model model structure
 */
//! [drawModel]
void prepareModel(GPUMemory&mem,CommandBuffer&commandBuffer,Model const&model){
  (void)mem;
  (void)commandBuffer;
  (void)model;
  /// \todo Tato funkce připraví command buffer pro model a nastaví správně pamět grafické karty.<br>
  /// Vaším úkolem je správně projít model a vložit vykreslovací příkazy do commandBufferu.
  /// Zároveň musíte vložit do paměti textury, buffery a uniformní proměnné, které buffer command buffer využívat.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace a v testech.
  
  mem.programs[0].fragmentShader = drawModel_fragmentShader;
  mem.programs[0].vertexShader = drawModel_vertexShader;

  mem.programs[0].vs2fs[0] = AttributeType::VEC3;
  mem.programs[0].vs2fs[1] = AttributeType::VEC3;
  mem.programs[0].vs2fs[2] = AttributeType::VEC2;
  mem.programs[0].vs2fs[3] = AttributeType::UINT;

  
  for(size_t i = 0; i < model.buffers.size(); ++i){
    mem.buffers[i].data = model.buffers[i].data;
    mem.buffers[i].size = model.buffers[i].size;
  }
  
  for(size_t i = 0; i < model.textures.size();++i){
    mem.textures[i] = model.textures[i];
  }

  // for( size_t i = 0; i < model.meshes.size(); ++i ){
  //   mem.uniforms[i].v4 = model.meshes[i].diffuseColor;
  // }


  addClearCommand(commandBuffer);

  glm::mat4 jednotkovaMatice = glm::mat4(1.f);
  for(size_t i = 0; i < model.roots.size();++i)
    prepareNode(mem,commandBuffer,model.roots[i],model,jednotkovaMatice);
}
//! [drawModel]

/**
 * @brief This function represents vertex shader of texture rendering method.
 *
 * @param outVertex output vertex
 * @param inVertex input vertex
 * @param si shader interface
 */
//! [drawModel_vs]
void drawModel_vertexShader(OutVertex&outVertex,InVertex const&inVertex,ShaderInterface const&si){
  (void)outVertex;
  (void)inVertex;
  (void)si;
  /// \todo Tato funkce reprezentujte vertex shader.<br>
  /// Vaším úkolem je správně trasnformovat vrcholy modelu.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.

   

  // pozice
  outVertex.attributes[0].v3 = si.uniforms[10+inVertex.gl_DrawID*5+0].m4 * glm::vec4(inVertex.attributes[0].v3,1.f);
  // normala
  outVertex.attributes[1].v3 = si.uniforms[10+inVertex.gl_DrawID*5+1].m4 * glm::vec4(inVertex.attributes[1].v3,0.f);
  // tex. koordinaty - poze se kopiruji
  outVertex.attributes[2].v2 = inVertex.attributes[2].v2;
  // cislo kresliciho prikazu gl_DrawID
  outVertex.attributes[3].u1 = inVertex.gl_DrawID;

  outVertex.gl_Position = si.uniforms[0].m4 * si.uniforms[10 + inVertex.gl_DrawID*5 + 0].m4 * glm::vec4(inVertex.attributes[0].v3,1.f);
}
//! [drawModel_vs]



/**
 * @brief This functionrepresents fragment shader of texture rendering method.
 *
 * @param outFragment output fragment
 * @param inFragment input fragment
 * @param si shader interface
 */
//! [drawModel_fs]
void drawModel_fragmentShader(OutFragment&outFragment,InFragment const&inFragment,ShaderInterface const&si){
  (void)outFragment;
  (void)inFragment;
  (void)si;
  /// \todo Tato funkce reprezentujte fragment shader.<br>
  /// Vaším úkolem je správně obarvit fragmenty a osvětlit je pomocí lambertova osvětlovacího modelu.
  /// Bližší informace jsou uvedeny na hlavní stránce dokumentace.
  
  float aF = 0.2f;
  glm::vec3 N = inFragment.attributes[1].v3;

  glm::vec3 KameraPozice = inFragment.attributes[0].v3 - si.uniforms[2].v3;
  if( si.uniforms[10 + inFragment.attributes[3].u1*5 + 4].v1 > 0.f && glm::normalize(N) == glm::normalize(KameraPozice) )
    N = -N;

  // pozice svetla -> pozice fragmentu vektor
  glm::vec3 L = si.uniforms[1].v3 - inFragment.attributes[0].v3;
  float dF = glm::clamp( glm::dot( L ,N ),0.f,1.f );


  glm::vec4 dC;
  if( si.uniforms[10 + inFragment.attributes[3].u1*5 + 3].i1 == -1 ){
    dC = si.uniforms[10 + inFragment.attributes[3].u1*5 + 2].v4;
  }
  else{
    dC = read_texture( si.textures[ si.uniforms[10 + inFragment.attributes[3].u1*5 + 3].i1 ] , inFragment.attributes[2].v2 );
  }
  

  glm::vec3 aL = dC*aF;
  glm::vec3 dL = dC*dF;

  outFragment.gl_FragColor = glm::vec4(aL + dL,dC.a);
}
//! [drawModel_fs]

