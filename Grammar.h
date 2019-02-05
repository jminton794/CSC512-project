#ifndef _GRAMMAR_H
#define _GRAMMAR_H
#include <fstream>
#include "Parser.h"
#include "Layer.h"


class Parser;

class Grammar
{
public:
	bool start();
	bool netParam();
	bool name();
	bool input();
	bool inputShape();
	bool inputShape0();
	bool BlobShape();
	bool dim();
	bool inputDim();
	bool inputDim0();
	bool force_backward();
	bool state();
	bool NetState();
	bool stage();
	bool stage0();
	bool level();
	bool debug_info();
	bool layer();
	bool layer0(Layer* aLayer);
	bool layerParam(Layer* aLayer);
	bool name(Layer* aLayer);
	bool type(Layer* aLayer);
	bool bottom(Layer* aLayer);
	bool bottom0(Layer* aLayer);
	bool top(Layer* aLayer);
	bool top0(Layer* aLayer);
	bool loss_weight(Layer* aLayer);
	bool loss_weight0(Layer* aLayer);
	bool propagate_down(Layer* aLayer);
	bool propagate_down0(Layer* aLayer);
	bool phase();
	bool phase(Layer* aLayer);
	bool param(Layer* aLayer);
	bool ParamSpec(Layer* aLayer);
	bool include(Layer* aLayer);
	bool exclude(Layer* aLayer);
	bool NetStateRule(Layer* aLayer);
	bool min_level(Layer* aLayer);
	bool max_level(Layer* aLayer);
	bool stage(Layer* aLayer);
	bool stage0(Layer* aLayer);
	bool not_stage(Layer* aLayer);
	bool not_stage0(Layer* aLayer);
	bool blobs(Layer* aLayer);
	bool blobs0(Layer* aLayer);
	bool BlobProto(Layer* aLayer);
	bool data(Layer* aLayer);
	bool data0(Layer* aLayer);
	bool diff(Layer* aLayer);
	bool diff0(Layer* aLayer);
	bool double_data(Layer* aLayer);
	bool double_data0(Layer* aLayer);
	bool double_diff(Layer* aLayer);
	bool double_diff0(Layer* aLayer);
	bool num(Layer* aLayer);
	bool channels(Layer* aLayer);
	bool height(Layer* aLayer);
	bool width(Layer* aLayer);
	bool share_mode(Layer* aLayer);
	bool lr_mult(Layer* aLayer);
	bool decay_mult(Layer* aLayer);
	bool other(Layer* aLayer);
	bool convolutionParam(Layer* aLayer);
	bool convolutionParam0(Layer* aLayer);
	bool batchNormParam(Layer* aLayer);
	bool batchNormParam0(Layer* aLayer);
	bool scaleParam(Layer* aLayer);
	bool scaleParam0(Layer* aLayer);
	bool reluParam(Layer* aLayer);
	bool reluParam0(Layer* aLayer);
	bool poolingParam(Layer* aLayer);
	bool poolingParam0(Layer* aLayer);
	bool concatParam(Layer* aLayer);
	bool concatParam0(Layer* aLayer);
	bool dropoutParam(Layer* aLayer);
	bool dropoutParam0(Layer* aLayer);
	bool reshapeParam(Layer* aLayer);
	bool reshapeParam0(Layer* aLayer);
	bool softmaxParam(Layer* aLayer);
	bool softmaxParam0(Layer* aLayer);
	bool num_output(Layer* aLayer);
	bool bias_term(Layer* aLayer);
	bool pad(Layer* aLayer);
	bool pad0(Layer* aLayer);
	bool kernel_size(Layer* aLayer);
	bool kernel_size0(Layer* aLayer);
	bool stride(Layer* aLayer);
	bool stride0(Layer* aLayer);
	bool dilation(Layer* aLayer);
	bool dilation0(Layer* aLayer);
	bool pad_h(Layer* aLayer);
	bool pad_w(Layer* aLayer);
	bool kernel_h(Layer* aLayer);
	bool kernel_w(Layer* aLayer);
	bool stride_h(Layer* aLayer);
	bool stride_w(Layer* aLayer);
	bool group(Layer* aLayer);
	bool weight_filler(Layer* aLayer);
	bool FillerParameter(Layer* aLayer);
	bool bias_filler(Layer* aLayer);
	bool filler(Layer* aLayer);
	bool engine(Layer* aLayer);
	bool axis(Layer* aLayer);
	bool force_nd_im2col(Layer* aLayer);
	bool value(Layer* aLayer);
	bool min(Layer* aLayer);
	bool max(Layer* aLayer);
	bool mean(Layer* aLayer);
	bool std(Layer* aLayer);
	bool sparse(Layer* aLayer);
	bool variance_norm(Layer* aLayer);
	bool use_global_stats(Layer* aLayer);
	bool moving_average_fraction(Layer* aLayer);
	bool eps(Layer* aLayer);
	bool num_axes(Layer* aLayer);
	bool negative_slope(Layer* aLayer);
	bool concat_dim(Layer* aLayer);
	bool dropout_ratio(Layer* aLayer);
	bool shape(Layer* aLayer);
	bool BlobShape(Layer* aLayer);
	bool dim(Layer* aLayer);
	bool pool(Layer* aLayer);
	bool global_pooling(Layer* aLayer);
	bool round_mode(Layer* aLayer);
	void instantiateParser(Parser* newParser);

private:
	Parser *parse;
	
public:
	std::ofstream *outFile;

	Grammar(std::ofstream *newOutFile)
	{
		outFile = newOutFile;
	}
	
};

#endif
