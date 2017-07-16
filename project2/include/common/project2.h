/**
 * @file project2.h
 * @brief Project 2 deliverables
 *
 * The primary deliverable for Project 2 is a character stream data processor.
 *
 * @author Jeff Schornick
 * @date 2017/07/06
**/

#ifndef __PROJECT2_H__
#define __PROJECT2_H__

/**
 * @brief Run Project 2 deliverables (data processor)
 *
 * This function runs the Project 2 deliverables, specifically a character
 * stream data processor. Depending on the target platform, a stream of data is
 * accepted either via serial UART or from the standard I/O device. The
 * processor collects statistics on the data stream and produces a summary,
 * which can be viewed over UART or standard out.
 *
 * @return void
**/
void project2(void);

#endif /* __PROJECT2_H__ */
